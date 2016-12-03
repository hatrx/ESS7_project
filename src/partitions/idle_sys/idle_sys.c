void idle_main(void)
{
    // A rather optimised nop loop. We have many nops per branch to better fill 
    // up each stage in the pipeline. It's possible this could be better optimised.
    asm(
        "loop:          \n\t"
        "NOP            \n\t"
        "NOP            \n\t"
        "NOP            \n\t"
        "NOP            \n\t"
        "NOP            \n\t"
        "B loop         \n\t"
    );
    // TODO: Optimise more!
}