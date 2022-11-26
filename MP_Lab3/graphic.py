import matplotlib.pyplot as plt


def read_file(filepath:str):
    with open(filepath) as file:
        points_list = file.readlines()
    return points_list


def create_points_lists(n: int, points_list: list):
    list = []
    for points in points_list:
        point = points.split(";")
        list.append(float(point[n]))
    return list


def create_acceleration_list(time_list: list):
    acceleration_list = []
    first_time = time_list[0]
    for time in time_list:
        acceleration_list.append(first_time / time)
    return acceleration_list

def create_efficiency_list(acceleration_list: list):
    efficiency_list = []
    for i, val in enumerate(acceleration_list):
        efficiency_list.append(val/(i+1))
    return efficiency_list


def show_graph():
    points_list = read_file("points.txt")
    thread_num_list = create_points_lists(0, points_list)
    time_list = create_points_lists(1, points_list)
    acceleration_list = create_acceleration_list(time_list)
    efficiency_list = create_efficiency_list(acceleration_list)

    #subplot 1
    plt.figure(figsize=(12, 7))
    plt.plot(thread_num_list, time_list, marker="o", c="g")
    plt.title(r'Time graph')
    plt.xlabel("Num threads", fontsize=14)
    plt.ylabel("Time, " + r'$mcs$', fontsize=14)
    plt.grid(True)

    #subplot 2
    plt.subplot(111)
    plt.figure(figsize=(12, 7))
    plt.plot(thread_num_list, acceleration_list, marker="o", c="r")
    plt.title(r'Acceleration graph')
    plt.xlabel("Num threads", fontsize=14)
    plt.ylabel("Acceleration", fontsize=14)
    plt.grid(True)

    #subplot 3
    plt.subplot(111)
    plt.figure(figsize=(12, 7))
    plt.plot(thread_num_list, efficiency_list, marker="o", c="b")
    plt.title(r'Efficiency graph')
    plt.xlabel("Num threads", fontsize=14)
    plt.ylabel("Efficiency", fontsize=14)
    plt.grid(True)

    plt.show()


def main():
    show_graph()


if __name__ == "__main__":
    main()
