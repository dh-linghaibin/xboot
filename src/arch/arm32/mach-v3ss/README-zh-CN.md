# 全志V3S，荔枝派(lichee-pi zero)

***

## 编译源码，生成的目标文件在output目录下
```shell
make clean
make CROSS_COMPILE=arm-linux-gnueabihf-  PLATFORM=arm32-v3ss
```

## 通过ssh发送到目标板
```shell
ssh root@192.168.1.110
```

## 运行
```shell
./xboot examples/games/2048/
```

# 相关网址

- XBOOT官方首页
  https://xboot.github.io/

- XBOOT源码(完全开源)，寄存在github，请直接`fork`一份代码，如需申请合并，直接`pull request`即可
  https://github.com/xboot/xboot

- XBOOT运行效果展示页面，首次接触的用户，建议观看，对XBOOT有个直观的感受
  https://github.com/xboot/xboot/blob/master/documents/xboot-show-case-zh-CN.md

- XBOOT系统开发指南
  https://github.com/xboot/xboot/blob/master/documents/xboot-system-development-guide-zh-CN.md

- XBOOT应用开发指南
  https://github.com/xboot/xboot/blob/master/documents/xboot-application-development-guide-zh-CN.md

- XBOOT官方QQ群，大佬聚集，请踊跃加入
  [658250248](https://jq.qq.com/?_wv=1027&k=5BOkXYO) (2000人)
