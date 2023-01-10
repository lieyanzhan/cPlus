import matplotlib.pyplot as plt
import numpy as np

X = np.linspace(-10, 10, 200)  # X轴坐标数据
Y = X - np.tan(X)
# Y = X - math.tan(X);                 # Y轴坐标数据
# plt.plot(X,Y,lable="$sin(X)$",color="red",linewidth=2)

plt.figure(figsize=(8,6))  # 定义图的大小
plt.xlabel("time(s)")     # X轴标签
plt.ylabel("Volt")        # Y轴坐标标签
plt.title("Example")      #  曲线图的标题

plt.plot(X,Y)            # 绘制曲线图
#在ipython的交互环境中需要这句话才能显示出来
plt.show()

