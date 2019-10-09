# randomscripts
## C

### [recursive_rename.c](https://github.com/MichelGeorgesNajarian/randomscripts/blob/master/C/recursive_rename.c)
Coded and tested on Ubuntu 18.04.1 "Bionic Beaver"

Compile the program using the included [Makefile](https://github.com/MichelGeorgesNajarian/randomscripts/blob/master/C/Makefile) by issuing the command:
`make` followed by:

`./recursive_rename $root_directory` (replace ``$root_directory`` with your specific directory like _anime_ for example) to run the program.

Its functionality is exactly the same as the script [recursive_rename.py](https://github.com/MichelGeorgesNajarian/randomscripts/blob/master/Python/recursive_rename.py) which is under the [Python directory](https://github.com/MichelGeorgesNajarian/randomscripts/blob/master/Python/)

`[Horriblesubs] Boku no Pico - 01[1080p].mkv` will be renamed to `Boku no Pico - 01.mkv`

`(some_random_fansub_group) Dragon_Ball_GT_42 [potato].mkv` will be renamed to `Dragon Ball GT 42.mkv`

and any trailling whitespaces after the extension will be deleted as well (Not yet implemented in the python equvalent script). For example, `[Horriblesubs] Boku no Pico - 01[1080p].mkv      ` will be renamed to `Boku no Pico - 01.mkv` (notice the extra whitespaces after the _.mkv_


## Python 

### [recursive_rename.py](https://github.com/MichelGeorgesNajarian/randomscripts/blob/master/Python/recursive_rename.py)
Coded and tested in Windows. ** Won't work in Linux since Windows uses '\' instead of '/' **

Its functionality is exactly the same as the program [recursive_rename.c](https://github.com/MichelGeorgesNajarian/randomscripts/blob/master/C/recursive_rename.C) which is under the [C directory](https://github.com/MichelGeorgesNajarian/randomscripts/blob/master/C/)

To run: if python.exe is in the PATH, just type:

`python.exe recursive_rename.py $root_directory` (replace `$root_directory` with your specific directory like _anime_ for example)

`[Horriblesubs] Boku no Pico - 01[1080p].mkv` will be renamed to `Boku no Pico - 01.mkv`

`(some_random_fansub_group) Dragon_Ball_GT_42 [potato].mkv` will be renamed to `Dragon Ball GT 42.mkv`
