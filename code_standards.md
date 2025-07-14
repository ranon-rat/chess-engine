We use Pascal case for naming public functions and classes.
If a function from a class is private then its cammel case.
example

```cpp
class A
{
private:
    int m_array[2]={};
public:
    A(){}
    int FirstValue()
    {
        return fromPosition(0);
    }
private:
    int fromPosition(size_t v)
    {   
        return m_array[v];
    }
};
```
Thats all you have to know :)


the code written for `game.h++` its just bullshit and i am not taking it seriously, but if you want to improve it then its something that you are welcomed to, i am just adding stuff there so i can easily  start testing the shit that this shit has to offer.
