import sys
import time

def main():
    if len(sys.argv) != 4:
        print("Usage: python3 idle_read.py path_to_file chunk_size timeout")
        sys.exit(-1)

    with open(sys.argv[1]) as f:
        while True:
            data = f.read(int(sys.argv[2]))
            print("[idle_read]", data)
            time.sleep(float(sys.argv[3]))
            if not data:
                break


if __name__ == '__main__':
    main()