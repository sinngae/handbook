Container:  
在音视频中的容器，一般指的是一种特定的文件格式，里面指明了所包含的音视频，字幕等相关信息

Stream:   
这个词有些微妙，很多地方都用到，比如TCP，SVR4系统等，其实在音视频，可以理解为单纯的音频数据或者视频数据等

Frame:   
这个概念不是很好明确的表示，指的是Stream中的一个数据单元，要真正对这个概念有所理解，可能需要看一些音视频编码解码的理论知识

Packet:   
是Stream的raw数据

Codec:   
encoder + decoder

解码流程  
+ 1 OPEN video_stream FROM video.avi
+ 2 READ packet FROM video_stream INTO frame
+ 3 IF frame NOT COMPLETE GOTO 2
+ 4 DO SOMETHING WITH frame
+ 5 GOTO 2
