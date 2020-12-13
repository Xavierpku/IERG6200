# key generation using depth and time
# d = get_dis_from_tof()
# t = get_current_time()
import numpy as np

MAX_N = 2000
d = 651
t = 13256

# get the prime list
is_prime = np.ones(MAX_N)
is_prime[1] = 0
is_prime[0] = 0

for i in range(2,MAX_N):
	for j in range(2*i,MAX_N,i):
		is_prime[j]=0

def find_nearest_prime(d):
	for i in range(d,2,-1):
		if is_prime[i]==1:
			dis_min = d-i
			nearest_prime = i
			break
	for i in range(d,MAX_N):
		if is_prime[i]==1:
			dis_max = i-d
			if(dis_max<dis_min):
				nearest_prime = i
			break
	return nearest_prime

d = np.int(np.round(d/10))
t = t%10000

d = find_nearest_prime(d)

key = pow(d,t) & 0xffffffffffffffff

print(key)