# SearchAlg
MFC与GDI+实现的搜索算法演示程序

简述：使用GDI完成了MFC中大部分绘制工作，使用GDI+产生搜索区域半透明效果，
程序演示了栅形搜索与回形搜索算法的路径和搜索区域，所有动画效果均可调。

本程序练习的技术：
1.MFC中多对话框之间数据的传递；
2.MFC中对EditControl，ComboBox等控件绑定成员变量，并控制它的显示，有效性等功能；
3.添加菜单栏以及如何进行相应的响应操作，菜单栏上的按键实际上和普通按钮差不多；
4.使用GDI+产生了半透明的效果，并得到了另一套绘制方法，它与GDI可以是兼容的；
5.引入了位图双缓冲技术，先在内存中创建位图，再将待刷新的图像直接在内存中创建，最后将内存中的位图
  copy到相应的位置或PictureControl中，避免了因对比度变化剧烈引起的刷新闪烁问题；
6.了解了在MFC的GDI中绘制图形时，CPen表明的是轮廓线，CBrush表明的是填充图样，可用CDC进行载入再绘制就可以得到想要的图形；
  对应GDI+中则直接调用Gdiplus::Graphics.DrawEllipse与FillEllipse进行轮廓线和图样填充，对于相应的Pen和SolidBrush都可以
  创建Color(Alpha,R,G,B)，即含透明度的颜色；
7.移动鼠标到某区域可在对应位置显示相应坐标，该判断准则是基于ClientRect与针对屏幕Rect之间的几何关系；
8.利用MFC中的OnTimer、SetTimer、KillTimer技术成功的产生了动画效果，并可调动画步进、动画绘制周期；
9.利用Photoshop中的ICO控件从jpg产生了png，最后保存为128×128的ICON图标文件，随后加载到MFC应用程序中使得exe图标以及左上角菜单栏图标随之改变；