import random

if __name__ == "__main__":

    with open("testdata.txt", 'w') as f:
        f.write("1\n")
        for i in range(1):
            edges = random.randint(3,1000)
            vertices = random.randint(3,250)
            f.write(f"{vertices} {edges}\n")
            for i in range(edges):
                f.write(f"{random.randint(1,vertices)} {random.randint(1,vertices)} {random.randint(1,9999)}\n")

