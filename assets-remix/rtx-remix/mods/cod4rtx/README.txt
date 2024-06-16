1) Download the base mod from here and extract the files into your cod4 root directory:
https://www.moddb.com/mods/cod4-mp-rtx-remix-iw3xo

2) Paste the contents of this folder into 'rtx-remix\mods\cod4rtx'
3) Open the 'mod.usda' and add the following to the top of the subLayer array:

    @./sp_killhouse.usda@,


    # like so:

    subLayers = [
        @./sp_killhouse.usda@,
        @./foliage.usda@,
        @./foliage_birch_test.usda@,
        .....
    ]

