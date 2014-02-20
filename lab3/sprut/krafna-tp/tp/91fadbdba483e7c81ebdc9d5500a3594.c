char proba3(void)
{
    x = 4;    // globalni x;
    if (0)
    {
        x = 6;   // globalni x;
        return 'a';
    } else {
        return (char)97;
//        return 97;            // int se ne moze implicitno u char castati
    }
}

