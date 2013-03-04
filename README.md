# RCC (RTree Compression Codec)

Welcome to RCC, the terribly inefficient lossy image compression codec!

RCC converts images into heightmaps for each channel (red, green, blue, alpha), then compresses these heightmaps.
Compression is achieved by grouping sets of nearby points into single rectangles, or RTrees.
In this way, RCC preserves details like facial features and text even at high compression levels.

## RCC In Action

| Original | Compressed | Compression Level |
|---|---|---|
|![](http://i.imgur.com/XK0B4.png)|![](http://i.imgur.com/GhwUc.png)|10|
|![](http://i.imgur.com/XK0B4.png)|![](http://i.imgur.com/4rRNK.png)|20|
|![](http://i.imgur.com/XK0B4.png)|![](http://i.imgur.com/54o3Q.png)|40|
|![](http://i.imgur.com/4KWWs.png)|![](http://i.imgur.com/ZuNxa.png)|2|
|![](http://i.imgur.com/4KWWs.png)|![](http://i.imgur.com/BbBFM.png)|5|
|![](http://i.imgur.com/SbEWg.png)|![](http://i.imgur.com/ZBn2i.png)|50|
|![](http://i.imgur.com/0sTgE.png)|![](http://i.imgur.com/ZuwYm.png)|10|
|![](http://i.imgur.com/0sTgE.png)|![](http://i.imgur.com/EwTrT.png)|20|
|![](http://i.imgur.com/0sTgE.png)|![](http://i.imgur.com/4UFC4.png)|30|

## Installation

```bash
$ svn checkout http://rtree-compression-codec.googlecode.com/svn/trunk/ rcc
$ cd rcc
$ make all
$ ./rcc [input-file-name.bmp]
```

## Usage

```
$ rcc [input-file-name.bmp]

-h
    show this documentation/help text
  
-m [compression level (int)]
    set the compression level (0 is lossless, higher values mean more compression, default is 5)
  
-o [filename]
    specify an output file name (default is "out")
```

## Authors

 * **Lane Aasen** [*github*](http://github.com/aaasen)
 * **Eamon Gaffney** [*github*](http://github.com/negaffney)
 * **Dylan Swiggett** [*github*](http://github.com/dylanswiggett)


```
  ______________________
 |  ____                | * * * * * * * * * * * * *
 | |    |               |   _         __      __  *
 | | ___|________       |  ||\\      / _|    / _| *
 | ||   |        |      |  || \\    / /     / /   *
 | ||___|  ___   |      |  || //   | |     | |    *
 |  |     |   |  |      |  ||/\\    \ \_    \ \_  *
 |  |     |___|  |      |  ||  \\    \__|    \__| *
 |  |____________|      |                         *
 |______________________| * * * * * * * * * * * * *
```
