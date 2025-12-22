import sys
import time
import threading

def reader():
    while True:
        data = sys.stdin.read(1)
        if data:
            print(f"[RX from QEMU] {data}", end="", flush=True)

def writer():
    while True:
        msg = input("\n[TX to QEMU] Enter text: ")
        for c in msg + "\n":
            sys.stdout.write(c)
            sys.stdout.flush()
            time.sleep(0.05)

if __name__ == "__main__":
    print("UART Simulator started")
    print("Type text and see echo from Cortex-A")

    t = threading.Thread(target=reader, daemon=True)
    t.start()

    writer()
