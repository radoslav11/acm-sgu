def main():
    _ = int(input())
    p = int(input())
    x = list(map(int, input().split()))
    
    ans = 0
    for v in x:
        ans += max(0, v ** p)
        
    print(ans)
    

if __name__ == "__main__":
    T = 1
    for _ in range(T):
        main()
