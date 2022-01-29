# dice-mosaic
dice-mosaic  -  Generate dice mosaics from jpeg/png images. Written in C.
                GNU General Public License v3.0.

Options
---
`-i [filename]`
>Input image(jpeg/png) filename or path.

>Default 'input.jpg'

`-o [filename]`
>Output image filename or path. Output format is .jpg.

>Default 'output.jpg`

`-s [number]`
>Factor to scale input image by. Must be greater than 0. After scaling, 1 pixel = 1 dice.

>Default is 0.05
                    
>Cannot be used in conjunction with `-w` or `-h`.

`-w [number]`
>Alternative scaling option, will scale an image to a specified number of dice in width while maintaining aspect ratio. Must be an integer greater than 0.

>Cannot be used in conjunction with `-s` or `-h`.

`-h [number]`
>Alternative scaling option, will scale an image to a specified number of dice in height while mainataining aspect ratio. Must be an integer greater than 0.

>Cannot be used in conjunction with `-s` or `-w`.

