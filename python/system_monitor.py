 import psutil
import serial
import json
import time
import subprocess

def get_gpu_temp():
    try:
        output = subprocess.check_output(['nvidia-smi', '--query-gpu=temperature.gpu', '--format=csv,noheader'], text=True)
        return int(output.strip())
    except:
        return 0

def get_music_info():
    # 示例：通过 Windows 媒体会话 API 获取，简化版返回固定字符串
    return {"title": "Unknown", "artist": "Unknown"}

def main():
    ser = serial.Serial('COM3', 115200, timeout=1)   # 根据实际端口修改
    while True:
        data = {
            "cpu_percent": psutil.cpu_percent(),
            "memory_percent": psutil.virtual_memory().percent,
            "gpu_temp": get_gpu_temp(),
            "music_info": get_music_info()
        }
        ser.write((json.dumps(data) + '\n').encode())
        time.sleep(1)

if __name__ == '__main__':
    main()
