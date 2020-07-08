#! python3
#coding=utf-8

"""
Python版本：3.x

计算不同配置下呼吸灯的周期

运行方式：
在命令行中输入：python count.py 

运行结果：
打印出当前配置中一个呼吸周期的时间
"""


#PWM点数
POINT_NUM = 180

#周期倍数
PERIOD_CLASS = 1

#幅值等级
AMPLITUDE_CLASS = 256

#定时器定时周期
TIMER_TIM_Period = 2**9

#APB1定时器分频
TIMER_TIM_Prescaler1 = 11
#APB2定时器分频
TIMER_TIM_Prescaler2 = 22

#STM32系统时钟频率和周期
f_hclk = 168000000

#定时器计时频率
f_pclk1 = f_hclk/2
t_pclk1 = 1/f_pclk1

f_pclk2 = f_hclk
t_pclk2 = 1/f_pclk2

#APB1定时器update事件周期
t_timer1 = t_pclk1*TIMER_TIM_Prescaler1*TIMER_TIM_Period

#APB1每个PWM点的时间
T_Point1 = t_timer1 * PERIOD_CLASS * AMPLITUDE_CLASS

#APB1整个呼吸周期
T_Up_Down_Cycle1 = T_Point1 * POINT_NUM

print ("APB1总线定时器呼吸周期：",T_Up_Down_Cycle1)


#APB2定时器update事件周期
t_timer2 = t_pclk2*TIMER_TIM_Prescaler2*TIMER_TIM_Period

#APB2每个PWM点的时间
T_Point2 = t_timer2 * PERIOD_CLASS * AMPLITUDE_CLASS

#APB2整个呼吸周期
T_Up_Down_Cycle2 = T_Point2 * POINT_NUM

print ("APB2总线定时器呼吸周期：",T_Up_Down_Cycle2)




