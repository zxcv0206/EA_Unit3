#include<bits/stdc++.h>
using namespace std;

#define RIVER 0
#define RIVERSTONE 1
#define ROCK 2
#define GRASS 3
#define MOUNTAIN 4

const int L = 20; // image size
const int N = 100; // population
const int G = 1000; // generation
const int K = 10;
const int T = 1;

int pool[N][L][L], child[N][L][L];
bool vis[L][L];
int best, comp[N];
double best_fit, fit[N];
int dirs[9][2] = {{0,1}, {0,-1}, {-1, 0}, {1,0}, {0,0}, {1,1}, {-1,-1}, {-1,1}, {1,-1}};



void init() {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < L; j++) {
            for(int k = 0; k < L; k++) {
                pool[i][j][k] = rand() % 5;
            }
        }
    }
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

void replace() {
    for(int i = 0; i < N; i++) {
        if(i == best) continue;
        for(int j = 0; j < L; j++) {
            for(int k = 0; k < L; k++) {
                pool[i][j][k] = child[i][j][k];
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
            if (fit[cand] > mx) {
                mx = fit[cand];
                p1 = cand;
            }
        }
        mx = INT_MIN;
        for(int k = 0; k < K; k++) {
            int cand = rand() % N;
            if (fit[cand] > mx) {
                mx = fit[cand];
                p2 = cand;
            }
        }
        crossover(i, p1, p2);
    }
    replace();
}

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
    if(pool[n][x][y] == ROCK) res++;
    for(int i = 0; i < 4; i++) {
        int X = x + dirs[i][0], Y = y + dirs[i][1];
        if(is_able(n, X, Y) && same_type(type, pool[n][X][Y])) res += DFS(type, n, X, Y, sz);
    }
    return res;
}

void cal_fit() {
    best_fit = INT_MIN;
    for(int i = 0; i < N; i++) {
        fit[i] = 0;
        comp[i] = 0;
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
                    int cnt_rock = DFS(pool[i][j][k], i, j, k, sz);
                    // if(pool[i][j][k] == GRASS || pool[i][j][k] == ROCK || pool[i][j][k] == MOUNTAIN) {
                        comp[i]++;
                        // fit[i] -= max(0, 1.0/6*sz - cnt_rock);
                        // fit[i] -= max(0.0, cnt_rock-0.25*sz);
                        // fit[i] += sz;
                    // }
                    mx_sz = max(sz, mx_sz);
                    mn_sz = min(sz, mn_sz);
                }
            }
        }

        fit[i] -= 100*abs(comp[i]-5);
        fit[i] -= (mx_sz - mn_sz);
        
        if (fit[i] > best_fit) {
            best_fit = fit[i];
            best = i;
        }
// cout << mx_sz << " " << mn_sz << endl;
    }
}

void mutation() {
    for(int i = 0; i < N; i++) {
        if(best == i) continue;
        if(comp[i] < 5)  {
            while(1) {
                int x = rand() % L, y = rand() % L;
                if (pool[i][x][y] == RIVER || pool[i][x][y] == RIVERSTONE) {
                    // int z = rand() % 4;
                    // int X = x+dirs[z][0], Y = y+dirs[z][1];
                    // if(X >= 0 && X < L && Y >= 0 && Y < L) {
                    //     pool[i][X][Y] = RIVER;
                    //     break;
                    // }
                    for(int k = 0; k < 9; k++) {
                        int X = x+dirs[k][0], Y = dirs[k][1];
                        if(X >= 0 && X < L && Y >= 0 && Y < L) {
                            pool[i][X][Y] = RIVER;
                        }
                    }
                    break;
                }
            }
            
        }
        if(comp[i] > 5)  {
            // int x = rand() % L, y = rand() % L;
            // for(int k = 0; k < 9; k++) {
            //     int X = x+dirs[k][0], Y = dirs[k][1];
            //     pool[i][x][y] = GRASS;
            //     if(X >= 0 && X < L && Y >= 0 && Y < L) {
            //         int z = rand() % 10;
            //         if(z <= 6) pool[i][X][Y] = GRASS;
            //         else if (z == 9) pool[i][X][Y] = MOUNTAIN;
            //         else pool[i][X][Y] = ROCK;
            //     }
            // }
            while(1) {
                int x = rand() % L, y = rand() % L;
                if (pool[i][x][y] == GRASS || pool[i][x][y] == MOUNTAIN || pool[i][x][y] == ROCK) {
                    for(int k = 0; k < 9; k++) {
                        int X = x+dirs[k][0], Y = dirs[k][1];
                        if(X >= 0 && X < L && Y >= 0 && Y < L) {
                            int z = rand() % 10;
                            if(z <= 6) pool[i][X][Y] = GRASS;
                            else if (z == 9) pool[i][X][Y] = MOUNTAIN;
                            else pool[i][X][Y] = ROCK;
                        }
                    }
                    break;
                }
            }
        }
    }
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
        cal_fit();
        for(int g = 1; g < G; g++) {
            run();
            cal_fit();
            mutation();
            cal_fit();
        }
        print(t);
        cout << comp[best] << endl;
        cout << best_fit << endl;
    }

    system("python3 gen_image.py");
}