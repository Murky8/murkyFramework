#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.

class InputDevices
{
public:
    // forward declaration
    enum class KeyCode;
    
    // methods
	InputDevices(HWND hWnd);
    static bool keyStatus(InputDevices::KeyCode iKey);
    void	processWindowsMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool	consumeAllMouseDx(int& out);
	bool	consumeAllMouseDy(int& out);
    // data
    u32 mouseB0 = 0;
    u32 mouseB1 = 0;
    u32 mouseB2 = 0;

    enum class KeyCode
    {
        enter = 0x0d,
        shift = 0x10,
        control = 0x11,
        esc = 0x1b,
        left = 0x25,
        up = 0x26,
        right = 0x27,
        down = 0x28,
        space = 0x29,
        num0 = 0x30,
        num1 = 0x31,
        num2 = 0x32,
        num3 = 0x33,
        num4 = 0x34,
        num5 = 0x35,
        num6 = 0x36,
        num7 = 0x37,
        num8 = 0x38,
        num9 = 0x39,
        a = 0x41,
        b = 0x42,
        c = 0x43,
        d = 0x44,
        e = 0x45,
        f = 0x46,
        g = 0x47,
        h = 0x48,
        i = 0x49,
        j = 0x4a,
        k = 0x4b,
        l = 0x4c,
        m = 0x4d,
        n = 0x4e,
        o = 0x4f,
        p = 0x50,
        q = 0x51,
        r = 0x52,
        s = 0x53,
        t = 0x55,
        u = 0x55,
        v = 0x56,
        w = 0x57,
        x = 0x58,
        y = 0x59,
        z = 0x5a,

        //joy_a   = 0x1000,
        //joy_b   = 0x1001,
        //joy_x   = 0x1002,
        //joy_y   = 0x1003,

        //mouse_lb= 0x2000,
    };

private:
	// data
	boost::circular_buffer<int> mouseDx{ 10 };
	boost::circular_buffer<int> mouseDy{ 10 };
	bool firstRun = true;

	int		consumeSingleMouseMove(boost::circular_buffer<int> &buffer);
    bool	consumeSingleMouseMove(int &out, boost::circular_buffer<int> &buffer);
	bool	consumeAllMouseMove(int &out, boost::circular_buffer<int> &buffer);
	int		consumeAllMouseMove(boost::circular_buffer<int>& buffer);
};