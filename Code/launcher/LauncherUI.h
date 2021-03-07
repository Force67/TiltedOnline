#pragma once

class SharedWindow;
class SharedPipeline;

class LauncherUI
{
public:
    LauncherUI(SharedWindow &aWindow, SharedPipeline &aPipeline);
    ~LauncherUI();

    void Draw();


  private:
    SharedWindow& m_Window;
    SharedPipeline& m_Pipeline;
};
