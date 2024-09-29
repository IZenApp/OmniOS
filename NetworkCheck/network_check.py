import platform
import subprocess

def check_network():
    os_type = platform.system()
    
    if os_type == "Windows":
        command = ["ping", "8.8.8.8", "-n", "1"]
    else:
        command = ["ping", "8.8.8.8", "-c", "1"]
    
    try:
        output = subprocess.check_output(command, stderr=subprocess.STDOUT)
        print("Сеть доступна")
    except subprocess.CalledProcessError:
        print("Сеть недоступна")

if __name__ == "__main__":
    check_network()