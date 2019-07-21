import math

radius = float(input('请输入圆的半径：'))

primeter = 2*math.pi*radius

area = math.pi*(radius**2)

print('周长：%.2f' % primeter)
print('面积：%.2f' % area)

