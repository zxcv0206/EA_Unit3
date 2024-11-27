#include<bits/stdc++.h>
using namespace std;

#define RIVER 0
#define RIVERSTONE 1
#define ROCK 2
#define GRASS 3
#define MOUNTAIN 4

const int L = 20; // image size
const int N = 100; // population
const int G = 5000; // generation
const int K = 10;
const int T = 10;

int pool[N][L][L], child[N][L][L];
bool vis[L][L];
int best, pool_comp[N], child_comp[N];
double best_fit = INT_MIN, pool_fit[N], child_fit[N];
int dirs[9][2] = {{0,1}, {0,-1}, {-1, 0}, {1,0}, {0,0}, {1,1}, {-1,-1}, {-1,1}, {1,-1}};


bool is_able(int n, int x, int y) {
    return !vis[x][y] && x >= 0 && x < L && y >= 0 && y < L;
}
bool same_type(int type, int x) {
    if (type == RIVER || type == RIVERSTONE) {
        if (x == RIVER || x == RIVERSTONE) return true;
        else return false;
    }  else {
        if (x == RIVER || x == RIVERSTONE) return false;
        else return true;
    }
}

int DFS(int type, int n, int x, int y, int &sz) {
    vis[x][y] = 1;
    sz++;
    int res = 0;
    if(child[n][x][y] == ROCK) res++;
    for(int i = 0; i < 4; i++) {
        int X = x + dirs[i][0], Y = y + dirs[i][1];
        if(is_able(n, X, Y) && same_type(type, child[n][X][Y])) res += DFS(type, n, X, Y, sz);
    }
    return res;
}


void cal_fit() {
    for(int i = 0; i < N; i++) {
        child_fit[i] = 0;
        child_comp[i] = 0;
        int  mx_sz = 0, mn_sz = INT_MAX;
        for(int j = 0; j < L; j++) {
            for(int k = 0; k < L; k++) {
                vis[j][k] = 0;
            }
        }

        for(int j = 0; j < L; j++) {
            for(int k = 0; k < L; k++) {
                int sz = 0;
                if(is_able(i, j, k)) {
                    double cnt_rock = DFS(child[i][j][k], i, j, k, sz);
                    if(child[i][j][k] == GRASS || child[i][j][k] == ROCK || child[i][j][k] == MOUNTAIN) {
                        child_comp[i]++;
                        child_fit[i] -= max(0.0, 1.0/10*sz - cnt_rock);
                        child_fit[i] -= max(0.0, cnt_rock-1.0/6*sz);
                        child_fit[i] += sz;
                    }
                    mx_sz = max(sz, mx_sz);
                    mn_sz = min(sz, mn_sz);
                    if(sz < 4) child_fit[i] -= 50;
                }
            }
        }
        child_fit[i] -= 100*abs(child_comp[i]-5);
        child_fit[i] -= (mx_sz - mn_sz);

    }
}

void replace() {
    for(int i = 0; i < N; i++) {
        if(child_fit[i] < pool_fit[i]) continue;
        for(int j = 0; j < L; j++) {
            for(int k = 0; k < L; k++) {
                pool[i][j][k] = child[i][j][k];
                pool_fit[i] = child_fit[i];
                pool_comp[i] = child_comp[i];
                if(pool_fit[i] > best_fit) {
                    best_fit = pool_fit[i];
                    best = i;
                }
            }
        }
    }
}

void init() {
    best_fit = INT_MIN;
    for(int i = 0; i < N; i++) {
        pool_fit[i] = INT_MIN;
    } 
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < L; j++) {
            for(int k = 0; k < L; k++) {
                child[i][j][k] = rand() % 4;
            }
        }
    }
    cal_fit();
    replace();
}

void crossover(int pair_id, int p1, int p2) {
    int pos = rand() % L;
    for(int i = 0; i < pos; i++) {
        for(int j = 0; j < L; j++) {
            child[pair_id][i][j] = pool[p1][i][j];
            child[pair_id+1][i][j] = pool[p2][i][j];
        }
    }

    for(int i = pos; i < L; i++) {
        for(int j = 0; j < L; j++) {
            child[pair_id][i][j] = pool[p2][i][j];
            child[pair_id+1][i][j] = pool[p1][i][j];
        }
    }
}


void mutation() {
    for(int i = 0; i < N; i++) {
        if(child_comp[i] < 5)  {
            while(1) {
                int x = rand() % L, y = rand() % L;
                if(child[i][x][y] == RIVER || child[i][x][y] == RIVERSTONE) {
                    for(int j = x; j < L; j++) {
                        for(int k = y-2; k < y+2; k++) {
                            if(j >= 0 && j < L && k >= 0 && k < L) child[i][j][k] = RIVER;
                        }
                    }
                    break;
                }
            }
        }
        if(child_comp[i] > 5)  {
            while(1) {
                int x = rand() % L, y = rand() % L;
                // if(child[i][x][y] == GRASS || child[i][x][y] == ROCK) {
                    for(int j = x-3; j < x+3; j++) {
                        for(int k = y-3; k < y+3; k++) {
                            if(j >= 0 && j < L && k >= 0 && k < L) {
                                int z = rand() % 100;
                                if(z < 95) child[i][j][k] = GRASS;
                                else child[i][j][k] = ROCK;
                            }
                        }
                    }
                    break;
                // }
            }
        }
    }
}

void run() {
    for(int i = 0; i < N; i+=2) {
        int p1, p2;
        int mx = INT_MIN;
        for(int k = 0; k < K; k++) {
            int cand = rand() % N;
            if (pool_fit[cand] > mx) {
                mx = pool_fit[cand];
                p1 = cand;
            }
        }
        mx = INT_MIN;
        for(int k = 0; k < K; k++) {
            int cand = rand() % N;
            if (pool_fit[cand] > mx) {
                mx = pool_fit[cand];
                p2 = cand;
            }
        }
        crossover(i, p1, p2);
    }
    mutation();
}

void print(int t) {
    ofstream file("../tmp/"+to_string(t)+".txt");
    for(int i = 0; i < L; i++) {
        for(int j = 0; j < L; j++) {
            file << pool[best][i][j] << " ";
        }
        file << endl;
    }
    file.close();

}

int main(){
    srand(time(NULL));

    for(int t = 0; t < T; t++) {
        init();
        for(int g = 1; g < G; g++) {
            run();
            cal_fit();
            replace();
        }
        print(t);
        cout << best_fit << endl;
    }

    system("python3 gen_image.py");
}