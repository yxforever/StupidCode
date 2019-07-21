"""
公英制单位转换
"""

value = float(input('请输入长度：'))
unit = input('请输入单位（in/英寸，cm/厘米）：')

if unit == 'in' or unit == '英寸':
    print('%f英寸 = %f厘米'% (value,value*2.54))
elif unit == 'cm' or unit == '厘米':
    print('%f厘米 = %f英寸'% (value,value /2.54))
else:
    print('请输入正确的单位。')

