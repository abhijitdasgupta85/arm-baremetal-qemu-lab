import socket
import threading
import time

HOST = "127.0.0.1"
PORT = 5555

def reader(sock):
    """
    Read data from QEMU UART over TCP,
    accumulate until newline, then print full message.
    """
    rx_buf = ""

    while True:
        try:
            data = sock.recv(1)
            if not data:
                print("\n[INFO] QEMU disconnected")
                break

            ch = data.decode(errors="ignore")

            if ch == '\n' or ch == '\r':
                if rx_buf:
                    print(f"\n[RX from QEMU] {rx_buf}")
                    rx_buf = ""
            else:
                rx_buf += ch

        except Exception as e:
            print(f"\n[ERROR] RX failed: {e}")
            break


def writer(sock):
    """
    Read user input and send it to QEMU UART over TCP.
    No local echo of transmitted data.
    """
    print("\n[INFO] Type text and press ENTER to send to QEMU")

    while True:
        try:
            # Read a full line from the user (blocking)
            msg = input()

            # Ignore empty lines
            if not msg:
                continue

            # Send message with newline termination
            sock.sendall((msg + "\n").encode())

        except EOFError:
            print("\n[INFO] Input closed. Exiting writer.")
            break

        except Exception as e:
            print(f"\n[ERROR] TX failed: {e}")
            break



def main():
    print("UART TCP Simulator starting...")
    print(f"Connecting to QEMU UART at {HOST}:{PORT}")

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Retry loop (QEMU may start later)
    while True:
        try:
            sock.connect((HOST, PORT))
            break
        except ConnectionRefusedError:
            print("Waiting for QEMU UART...")
            time.sleep(1)

    print("Connected to QEMU UART")

    t = threading.Thread(target=reader, args=(sock,), daemon=True)
    t.start()

    writer(sock)


if __name__ == "__main__":
    main()
