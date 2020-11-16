void print_array(char **array, int count)
{
    printf("printArray\n");
    for (int i = 0; i < count; i++)
    {
        printf("%s\n", array[i]);
    }
}