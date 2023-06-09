/// @file Preferences.cpp
/// @author Alix ANNERAUD (alix.anneraud@outlook.fr)
/// @brief
/// @version 0.2
/// @date 29-06-2021
///
/// @copyright Copyright (c) 2021

#include "Preferences.hpp"

Preferences_Class::Preferences_Handle_Class Preferences_Class::Handle;

Preferences_Class::Preferences_Class(const Accounts_Types::User_Type *Owner_User) : Software_Class(Handle, Owner_User)
{
}

Preferences_Class::~Preferences_Class()
{
    Window.Delete();

    Communication.WiFi.Scan.Delete();
}

void Preferences_Class::Set_Interface()
{
    using namespace Graphics_Types;

    Window.Create(this);
    Window.Set_Title("Preferences");

    Tabs.Create(Window.Get_Body(), Direction_Type::Bottom, 48);
    Tabs.Set_Size(Percentage(100), Percentage(100));
    Tabs.Set_Style_Background_Opacity(Opacity_Type::Opacity_0_Percent, 0);
    Tabs.Add_Event(this, Graphics_Types::Event_Code_Type::Value_Changed);

    {
        Button_Matrix_Type Tab_Buttons = Tabs.Get_Tab_Buttons();
        //       Tab_Buttons.Set_Style_Background_Color(Color_Type::Grey[2], 0);
        //       Tab_Buttons.Set_Style_Text_Color(Color_Type::White, 0);
        //        Tab_Buttons.Set_Style_Border_Side(Border_Side_Type::Right, Part_Type::Items | Graphics_Types::State_Type::Checked);
        //        Tab_Buttons.Set_Style_Border_Color(Color_Type::White, Part_Type::Items | Graphics_Types::State_Type::Checked);
        //        Tab_Buttons.Set_Style_Text_Color(Color_Type::White, Part_Type::Items | Graphics_Types::State_Type::Checked);
    }

    Personal_Tab = Tabs.Add_Tab("Personal");
    Softwares_Tab = Tabs.Add_Tab("Softwares");
    Hardware_Tab = Tabs.Add_Tab("Hardware");
    Network_Tab = Tabs.Add_Tab("Wireless");
    Users_Tab = Tabs.Add_Tab("Users");
    System_Tab = Tabs.Add_Tab("System");

    Draw_Personal();
    Draw_Softwares();
    Draw_Users();
    Draw_Wireless();
    Draw_Hardware();
    Draw_System();
    
    Keyboard.Create(Window.Get_Body());
    Keyboard.Add_Flag(Flag_Type::Hidden);
    Keyboard.Add_Flag(Flag_Type::Floating);
}

void Preferences_Class::Main_Task_Function()
{
    Set_Interface();

    while (true)
    {
        if (this->Instruction_Available())
        {
            this->Execute_Instruction(this->Get_Instruction());
        }
        else
        {
            this->Main_Task.Delay(40);
        }
    }
}

void Preferences_Class::Execute_Instruction(Instruction_Type Instruction)
{
    if (Instruction.Get_Sender() == &Softwares)
    {
        if (Instruction.Softwares.Get_Code() == Softwares_Types::Event_Code_Type::Close)
        {
            delete this;
            return;
        }
    }
    else if (Instruction.Get_Sender() == &Graphics)
    {
        if (Instruction.Graphics.Get_Current_Target() == Tabs)
        {
            if (Instruction.Graphics.Get_Code() == Graphics_Types::Event_Code_Type::Value_Changed)
            {
                switch (Tabs.Get_Tab_Active())
                {
                case 0:
                    Refresh_Personal();
                    break;
                case 1:
                    Refresh_Softwares();
                    break;
                case 2:
                    Refresh_Hardware();
                    break;
                case 3:
                    Refresh_Wireless();
                    break;
                case 4:
                    Refresh_Users();
                    break;
                case 5:
                    Refresh_System();
                    break;
                }
                return;
            }
        }
        else if (Instruction.Graphics.Get_Code() == Graphics_Types::Event_Code_Type::Focused)
        {
            Graphics_Types::Text_Area_Type Text_Area = Instruction.Graphics.Get_Current_Target();
            if (Text_Area.Is_Valid())
                Keyboard.Set_Text_Area(Text_Area);
        }
        else if (Instruction.Graphics.Get_Code() == Graphics_Types::Event_Code_Type::Defocused)
        {
            Keyboard.Remove_Text_Area();
        }
        else
        {
            // -
            switch (Tabs.Get_Tab_Active())
            {
            case 0:
                Execute_Personal_Instruction(Instruction);
                break;
            case 1:
                Execute_Softwares_Instruction(Instruction);
                break;
            case 2:
                Execute_Hardware_Instruction(Instruction);
                break;
            case 3:
                Execute_Wireless_Instruction(Instruction);
                break;
            case 4:
                Execute_Users_Instruction(Instruction);
                break;
            case 5:
                Execute_System_Instruction(Instruction);
                break;
            }
        }
    }
}












