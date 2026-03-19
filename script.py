
import subprocess
import matplotlib.pyplot as plt

def run_simulation(num_users):
    stats = {}
    # Run the simulator with the specified number of users
    result = subprocess.run(["./simulate", str(num_users)], capture_output=True, text=True).stdout
    # print(result)
    # for line in result.split("\n")[:-1]:
    #     tmp = line.split(":")
    #     print(tmp)
    #     stats[tmp[0].strip()] = float(tmp[1].strip())
    res_time = result.split("\n")[0].split(":")[1].strip()
    stats["Average Response Time"] = float(res_time)
    tgpt= result.split("\n")[1].split(":")[1].strip()
    stats["Throughput"] = float(tgpt)
    stats["avg server utilization"] = float(result.split("\n")[6].split(":")[1].strip()[:-1])
    return stats
    
    
if __name__ == "__main__":
    num_users = 90  # You can change this value to test with different numbers of users
    restime = []
    tgpt = []
    utilization = []
    users = range(1, 2000, 100)
    for user in users:
        restime.append(run_simulation(user)["Average Response Time"])
        tgpt.append(run_simulation(user)["Throughput"])
        utilization.append(run_simulation(user)["avg server utilization"])
        
        
    plt.plot(users, restime)
    plt.xlabel("Number of Users")
    plt.ylabel("Average Response Time")
    plt.title("Performance of the System")
    plt.savefig("performance_plot.png")
    plt.clf()
    
    plt.plot(users, tgpt)
    plt.xlabel("Number of Users")
    plt.ylabel("Throughput")
    plt.title("Throughput of the System")
    plt.savefig("throughput_plot.png")
    plt.clf()
    
    prod=[]
    for i in range(len(users)):
        prod.append(restime[i]*tgpt[i])
    plt.plot(users, prod)
    plt.xlabel("Number of Users")
    plt.ylabel("Response Time * Throughput")
    plt.title("Performance Metric")
    plt.savefig("rs_throughput_plot.png")
    plt.clf()
        
    plt.plot(tgpt, utilization)
    plt.xlabel("Throughput")
    plt.ylabel("Server Utilization")
    plt.title("Server Utilization vs Throughput")
    plt.savefig("utilization_plot.png")
    plt.clf()
        