from PIL import Image

def create_image(grid, tile_images, tile_size, output_file, format="BMP"):
    """
    生成圖片並根據輸入網格和圖片填充，輸出為指定格式。
    
    :param grid: 二維陣列，表示每格對應的圖片索引。
    :param tile_images: 字典，鍵為網格值，值為對應的圖片路徑。
    :param tile_size: 每個圖片的大小（寬, 高）。
    :param output_file: 輸出檔案名稱。
    :param format: 輸出格式，BMP 或 PPM。
    """
    rows, cols = len(grid), len(grid[0])
    tile_width, tile_height = tile_size

    # 設定輸出圖片大小
    output_width = cols * tile_width
    output_height = rows * tile_height
    output_image = Image.new("RGB", (output_width, output_height))

    # 載入所有圖片並填補到輸出圖片中
    tile_images_cache = {}
    for row in range(rows):
        for col in range(cols):
            tile_value = grid[row][col]
            if tile_value not in tile_images_cache:
                # 載入圖片並縮放到指定大小
                tile_images_cache[tile_value] = Image.open(tile_images[tile_value]).resize((tile_width, tile_height))
            # 將圖片貼到對應格子位置
            output_image.paste(tile_images_cache[tile_value], (col * tile_width, row * tile_height))

    # 儲存輸出圖片
    output_image.save(output_file, format)
    print(f"Image saved as {output_file} in {format} format.")

def read_2d_array(file_path):
    with open(file_path, "r") as f:
        # 逐行讀取並將每行的數字轉換為列表
        array = [list(map(int, line.split())) for line in f]
    return array

# 範例使用
if __name__ == "__main__":
    # 定義每個值對應的圖片路徑
    tile_images = {
        0: "../image/river.png",  
        1: "../image/riverstone.png",
        2: "../image/rock.png",
        3: "../image/grass.png",
        4: "../image/mountain.png",
    }
    # 每格的大小
    tile_size = (50, 50)  # 每格 50x50 像素

    for t in range(1):
        grid = read_2d_array("../tmp/"+str(t)+".txt")

        # 輸出 BMP 格式圖片
        create_image(grid, tile_images, tile_size, "../output/"+str(t)+".bmp", format="BMP")
