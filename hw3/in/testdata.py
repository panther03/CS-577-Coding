import random

if __name__=="__main__":
    num_intervals = 10000
    with open("testdata.txt", 'w') as f:
        f.write("1\n"+str(num_intervals)+"\n")
        for _ in range(num_intervals):
            end = random.randint(1,200)
            start = random.randint(0,end-1)
            f.write(f"{start} {end}\n")