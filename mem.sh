###
 # @Author: Bedrock
 # @Date: 2022-08-11 17:55:50
 # @LastEditors: Bedrock
 # @LastEditTime: 2022-08-11 17:57:09
 # @Description: 
### 

mem_free='0'

mem_free1='0'
#MB为单位
mem_free=`free -m | grep "Mem" | awk '{print $3}'`
for i in {1..100}
do
        mem_free1=`free -m | grep "Mem" | awk '{print $3}'`
        if ((${mem_free1} > ${mem_free}))
        then mem_free=${mem_free1}
        fi
        sleep 0.1
done
echo "$now   ${mem_free}MB"