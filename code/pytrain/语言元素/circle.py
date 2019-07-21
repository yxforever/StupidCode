"""
圆的周长和面积
"""

import math

radius = float(input('请输入圆的半径：'))

#周长
perimeter = 2*math.pi*radius

#面积
area = math.pi*(radius**2)

print('周长：%.2f'% perimeter)
print('面积：%.2f'% area)

