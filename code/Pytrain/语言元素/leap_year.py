"""
闰年判断，是为true，否为false
"""

year = int (input('请输入年份：'))

leap_year = (year % 4 == 0 and year % 100 != 0
        or year % 400 == 0)
print(leap_year)

