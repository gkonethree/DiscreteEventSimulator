from simulator import Simulation

if __name__ == "__main__":

    user_levels = [1,5, 10 ,4000]

    for n in user_levels:
        sim = Simulation(n)
        result = sim.run()
        print(f"\nUsers: {n}")
        for k, v in result.items():
            print(k, ":", v)