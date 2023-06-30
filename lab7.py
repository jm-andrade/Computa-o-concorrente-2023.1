
from threading import Thread
import math
import time

class ThreadCalculaSeriesPi(Thread):
    def __init__(self, ini, num_series):
        super().__init__()
        self.ini = ini
        self.num_series = num_series
        self.total = 0.0

    def run(self):
        fim = self.ini + self.num_series
        for k in range(self.ini, fim):
            self.total += math.pow(-1, k) / (2 * k + 1)

def calc_aprox_pi_concorr(nseries, nthreads):
    tam_bloco = nseries // nthreads
    threads = []
    somas = []

    for n in range(nthreads):
        threads.append(ThreadCalculaSeriesPi(n * tam_bloco, tam_bloco))
        threads[n].start()
    
    for thread in threads:
        thread.join()
        somas.append(thread.total)

    return sum(somas) * 4

nseries = 1000000
nthreads = 4

t_ini = time.time()
pi = calc_aprox_pi_concorr(nseries, nthreads)
t_fim = time.time()

print("Minha aproximação de pi:", pi, "\nConstante pi de math.pi:", math.pi)
print("O calculo foi realizado com", nseries,"series, e", nthreads, "threads.")
print("Tempo de execucao:", t_fim - t_ini, " segundos")
