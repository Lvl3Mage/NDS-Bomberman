
u8 w_empty[64] =
{
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};
u8 w_full[64] =
{
    1,1,1,1,1,1,1,1,
    1,1,1,2,1,1,1,1,
    1,1,2,2,2,1,1,1,
    1,2,2,2,2,2,1,1,
    1,1,1,2,1,2,1,1,
    1,1,1,1,2,1,2,2,
    1,1,1,1,1,2,2,2,
    1,1,1,1,1,1,2,1
};
u8 w_halfV[64] =
{
    1,1,1,3,3,0,0,0,
    2,1,1,3,3,0,0,0,
    1,2,2,3,3,0,0,0,
    1,2,2,3,3,0,0,0,
    1,2,2,3,3,0,0,0,
    2,2,1,3,3,0,0,0,
    1,2,1,3,3,0,0,0,
    1,1,1,3,3,0,0,0,
};
u8 w_halfH[64] =
{
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    3,3,3,3,3,3,3,3,
    3,3,3,3,3,3,3,3,
    2,2,1,1,1,1,2,1,
    1,2,2,2,2,2,1,1,
    1,1,1,1,1,1,1,1,
};

u8 w_corner1[64]=
{
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    3,3,0,0,0,0,0,0,
    3,3,3,0,0,0,0,0,
    1,3,3,3,0,0,0,0,
    2,2,3,3,3,0,0,0,
    1,2,1,3,3,0,0,0
};
u8 w_fullCorner1[64]=
{
    1,1,1,1,1,1,1,1,
    1,1,1,2,1,2,1,1,
    1,1,1,2,2,2,2,1,
    3,3,1,2,2,2,2,1,
    3,3,3,1,2,1,2,1,
    0,3,3,3,1,1,2,1,
    0,0,3,3,3,1,2,1,
    0,0,0,3,3,1,1,1
};
u8 w_path[64]=
{
    1,1,1,3,3,0,0,0,
    1,2,1,3,3,0,0,0,
    1,2,1,2,3,3,0,0,
    3,3,1,2,2,3,3,3,
    3,3,3,1,2,1,3,3,
    0,3,3,3,1,2,2,1,
    0,0,3,3,3,1,2,1,
    0,0,0,3,3,1,1,1
};
u8 recolor[64]=
{
    4,4,4,4,4,4,4,4,
    4,1,0,0,0,0,0,4,
    4,1,1,0,0,0,0,4,
    4,1,1,1,0,0,0,4,
    4,1,1,1,1,0,0,4,
    4,1,1,1,1,1,0,4,
    4,1,1,1,1,1,1,4,
    4,4,4,4,4,4,4,4
};

u8 corner2[64] =
    {
        1,1,1,1,1,1,1,1,
        1,1,1,2,1,1,1,1,
        1,1,2,2,2,1,1,1,
        1,2,2,2,2,2,1,1,
        1,1,1,2,1,2,1,1,
        1,1,1,1,2,1,2,2,
        1,1,1,1,1,2,2,2,
        1,1,1,1,1,1,2,1
    };