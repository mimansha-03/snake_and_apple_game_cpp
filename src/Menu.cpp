#include "Menu.h"
#include "raylib.h"
#include "Animation.h"
#include <cmath>

Menu::Menu()
{
    items_ =
    {
        "Start Game",
        "Difficulty",
        "Quit"
    };

    selected_ = 0;
    result_ = MenuResult::None;
    difficultyMode_ = false;
    animationTime_ = 0.0f;
}

void Menu::setDifficultyMode(bool enable)
{
    difficultyMode_ = enable;

    selected_ = 0;

    if(enable)
    {
        items_ =
        {
            "Easy",
            "Medium",
            "Hard",
            "Back"
        };
    }
    else
    {
        items_ =
        {
            "Start Game",
            "Difficulty",
            "Quit"
        };
    }
}

void Menu::update()
{
    if (IsKeyPressed(KEY_ENTER))
    {
        if(!difficultyMode_)
        {
            switch(selected_)
            {
                case 0: result_ = MenuResult::Start; break;

                case 1: result_ = MenuResult::Difficulty; break;

                case 2: result_ = MenuResult::Quit; break;
            }
        }
        else
        {
            switch(selected_)
            {
                case 0: result_ = MenuResult::DifficultyEasy; break;

                case 1: result_ = MenuResult::DifficultyMedium; break;

                case 2: result_ = MenuResult::DifficultyHard; break;

                case 3: result_ = MenuResult::Back; break;
            }
        }
    }

    if (IsKeyPressed(KEY_DOWN))
    {
        selected_++;
        if(selected_ >= (int)items_.size())
            selected_ = 0;
    }

    if (IsKeyPressed(KEY_UP))
    {
        selected_--;
        if(selected_ < 0)
            selected_ = (int)items_.size() - 1;
    }
}

MenuResult Menu::consumeResult()
{
    MenuResult r = result_;
    result_ = MenuResult::None;
    return r;
}       

void Menu::draw() const
{
    animationTime_ += GetFrameTime();
    
    int centerX = GetScreenWidth()/2;

    Color grid = Fade(Color{70,90,110,255},0.18f);

    for(int x=0;x<GetScreenWidth();x+=40)
        DrawLine(x,0,x,GetScreenHeight(),grid);

    for(int y=0;y<GetScreenHeight();y+=40)
        DrawLine(0,y,GetScreenWidth(),y,grid);

    for(int i=0;i<60;i++)
    {
        float x = fmod(i*97 + animationTime_*10,(float)GetScreenWidth());
        float y = fmod(i*57 + animationTime_*4,(float)GetScreenHeight());

        DrawCircleV({x,y},1.5f,Fade(SKYBLUE,0.45f));
    }

    float move = sinf(animationTime_ * 1.8f) * 20;

    float sx = centerX - 180 + move;

    for(int i = 1; i < 18; i++)
    {
        float t = i * 0.35f + animationTime_ * 2.5f;

        float x = sx + i * 22;
        float y = 45 + sinf(t) * 18;

        Color c =
        {
            70,
            (unsigned char)(245 - i * 5),
            110,
            255
        };

        DrawCircle(x, y, 9, c);
    }

    DrawCircle(
        sx,
        45 + sinf(animationTime_ * 2.5f) * 18,
        11,
        Color{120,255,120,255}
    );


    float titleY = 80 + Animation::Bounce(2.5f, 6);
    DrawText(
        "SNAKE & APPLE",
        centerX - MeasureText("SNAKE & APPLE", 60) / 2,
        (int)titleY,
        60,
        Color{245,245,255,255});

    DrawText(
        "Modern C++ Edition",
        centerX - MeasureText("Modern C++ Edition",22)/2,
        150,
        22,
        Color{150, 170, 210, 255});

    DrawLine(centerX-240,178,centerX-90,178,Color{90,110,130,255});
    DrawLine(centerX+90,178,centerX+240,178,Color{90,110,130,255});
    DrawCircle(centerX-250,178,3,LIME);
    DrawCircle(centerX+250,178,3,LIME);

    const int MENU_START_Y = difficultyMode_ ? 215 : 225;
    constexpr int MENU_SPACING = 55;

    int y = MENU_START_Y;

    for(size_t i=0;i<items_.size();i++)
    {
        Color c;
        if(i==selected_) {
            float glow = Animation::Pulse(7);
            c = Color{ 255, (unsigned char)(190 + glow*65), 40, 255};
        } else {
            c = Color{220,220,220,255};
        }

        std::string text;

        if (i == selected_) {
            text = items_[i];

            DrawText(
                ">",
                centerX-100,
                y,
                32,
                GOLD
            );
        }
        else {
            text = items_[i];
        }

        int offset = (i == selected_) ? (int)Animation::Wave(9, 5) : 0;

        DrawText(
            text.c_str(),
            centerX - MeasureText(text.c_str(),30)/2 + 20 + offset,
            y,
            30,
            c
        );

        y += MENU_SPACING;
    }

    if (!difficultyMode_)
    {
        const int panelY = 420;
        
        DrawText(
            "Press ENTER to Start",
            centerX - MeasureText("Press ENTER to Start",22)/2,
            panelY - 28,
            22,
            Color{255,220,70,255}
        );
    }

    if (!difficultyMode_) {

        const int panelY = 420;
        
        Rectangle panel =
        {
            120,
            (float)panelY,
            (float)GetScreenWidth() - 240,
            190
        };

        DrawRectangleRounded(
            panel,
            0.04f,
            8,
            Fade(BLACK,0.45f)
        );

        DrawRectangleRoundedLinesEx(
            panel,
            0.04f,
            8,
            2,
            Color{90,110,130,255}
        );

        DrawText("HOW TO PLAY", centerX - MeasureText("HOW TO PLAY", 30) / 2, panelY + 20, 30, LIME);

        DrawText(
            "Move",
            centerX - 220,
            panelY + 60,
            24,
            LIME
        );

        DrawText(
            ": W A S D  or  Arrow Keys",
            centerX - 120,
            panelY + 60,
            24,
            RAYWHITE
        );

        DrawText(
            "Pause",
            centerX - 220,
            panelY + 100,
            24,
            LIME
        );

        DrawText(
            ": P",
            centerX - 120,
            panelY + 100,
            24,
            RAYWHITE
        );

        DrawText(
            "Quit",
            centerX - 220,
            panelY + 140,
            24,
            LIME
        );

        DrawText(
            ": Q",
            centerX - 120,
            panelY + 140,
            24,
            RAYWHITE
        );
    }
}