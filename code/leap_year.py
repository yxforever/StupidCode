"""
判断闰年
"""
import math

year = int (input('请输入年份：'))
leap_year = (year % 4 == 0 and year % 400 != 0
        or year %  400 ==0)
print(leap_year)

