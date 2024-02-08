#pragma once

class Context;

namespace Editor
{
    class AxisSelectSystem final
    {
    public:
        static bool Update(Context *);
    };
    class EOSelectSystem final
    {
    public:
        static void Update(Context *);
    };
}