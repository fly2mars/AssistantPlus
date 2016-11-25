/*
Console.h
Written by Matthew Fisher

Functions for interacting with the console.

*/

namespace Console
{
    std::ofstream& File();
    void AdvanceLine();
    void WriteLine(const suString &S);
    void OverwriteLine(const suString &S);
    void WriteString(const suString &S);

}
