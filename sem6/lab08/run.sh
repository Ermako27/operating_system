sudo insmod wqueue.ko

echo ">>>>> showing /proc/interrupts"

cat /proc/interrupts

echo ">>>>> press button..."

sleep 2.0

dmesg | tail -20

sleep 0.5

sudo rmmod wqueue.ko



