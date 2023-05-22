/// @file System.cpp
/// @author Alix ANNERAUD (alix@anneraud.fr)
/// @brief
/// @version 0.1.0
/// @date 06-05-2023
///
/// @copyright Copyright (c) 2023

#include "Preferences.hpp"

void Preferences_Class::Refresh_System()
{
    Static_String_Type<32> Temporary_String;
    System.Get_Device_Name(Temporary_String);
    System_Device_Name_Text_Area.Set_Text(Temporary_String);

    Temporary_String = "";
    System.Get_NTP_Server(Temporary_String);
    System_Time_NTP_Server_Text_Area.Set_Text(Temporary_String);

    Temporary_String = "";

    for (uint8_t i = -12; i <= 12; i++)
    {

        if ((i == -4) || (i == -3) || (i == 3) || (i == 4) || (i == 5) || (i == 6) || (i == 9))
        {
            if ((i * 60 * 60) >= (System.Get_UTC_Offset() - (30 * 60)))
            {
                System_Time_Zone_Roller.Set_Selected(i + 12, false);
                break;
            }
        }
        else
        {
            if ((i * 60 * 60) >= System.Get_UTC_Offset())
            {
                System_Time_Zone_Roller.Set_Selected(i + 12, false);
                break;
            }
        }
    }

    System_Time_Daylight_Offset_Spinbox.Set_Value(System.Get_Daylight_Offset());
}

void Preferences_Class::Execute_System_Instruction(const Instruction_Type &Instruction)
{
    if (Instruction.Graphics.Get_Target() == System_Device_Apply_Button)
    {
        if (Instruction.Graphics.Get_Code() == Graphics_Types::Event_Code_Type::Clicked)
        {
            Static_String_Type<32> Temporary_String = System_Device_Name_Text_Area.Get_Text();
            System.Set_Device_Name(Temporary_String);
        }
    }
    else if (Instruction.Graphics.Get_Target() == System_Time_Apply_Button)
    {
        if (Instruction.Graphics.Get_Code() == Graphics_Types::Event_Code_Type::Clicked)
        {
            Static_String_Type<32> Temporary_String = System_Time_NTP_Server_Text_Area.Get_Text();
            System.Set_NTP_Server(Temporary_String);
            // TODO : System.Set_Time_Zone(..);
        }
    }
    else if (Instruction.Graphics.Get_Target() == System_Time_Minus_Button)
    {
        if (Instruction.Graphics.Get_Code() == Graphics_Types::Event_Code_Type::Short_Clicked || Instruction.Graphics.Get_Code() == Graphics_Types::Event_Code_Type::Long_Pressed_Repeat)
        {
            System_Time_Daylight_Offset_Spinbox.Decrement();
        }
    }
    else if (Instruction.Graphics.Get_Target() == System_Time_Plus_Button)
    {
        if (Instruction.Graphics.Get_Code() == Graphics_Types::Event_Code_Type::Short_Clicked || Instruction.Graphics.Get_Code() == Graphics_Types::Event_Code_Type::Long_Pressed_Repeat)
        {
            System_Time_Daylight_Offset_Spinbox.Increment();
        }
    }
}

void Preferences_Class::Draw_System()
{
    using namespace Graphics_Types;

    Log_Verbose("System", "Draw 1");

    System_Tab.Set_Flex_Flow(Flex_Flow_Type::Row);
    System_Tab.Set_Style_Pad_All(0, 0);
    System_Tab.Clear_Flag(Flag_Type::Scroll_Elastic);
    System_Tab.Clear_Flag(Flag_Type::Scroll_Momentum);

    Log_Verbose("System", "Draw 2");

    // - Grid layout

    System_Tab.Set_Grid_Descriptor_Array(Column_Descriptor, Row_Descriptor);
    System_Tab.Set_Style_Pad_All(10, 0);

    Log_Verbose("System", "Draw 3");

    // - - Device section

    const uint8_t Device_Section_Row = 0;

    {
        Log_Verbose("System", "Draw 4");

        // - - - Device title label
        {
            Label_Type Label;
            Label.Create(System_Tab, "Device");
            Label.Set_Style_Text_Font(&Graphics.Get_Font(24), 0);
            Label.Set_Grid_Cell(Grid_Alignment_Type::Center, 0, 8, Grid_Alignment_Type::Center, Device_Section_Row, 1);
        }

        Log_Verbose("System", "Draw 5");

        // - - - Apply button
        System_Device_Apply_Button.Create(System_Tab, "Apply", 0, 0, this);
        System_Device_Apply_Button.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 6, 2, Grid_Alignment_Type::Stretch, Device_Section_Row, 1);

        Log_Verbose("System", "Draw 6");

        // - - - Device name text area
        System_Device_Name_Text_Area.Create(System_Tab);
        System_Device_Name_Text_Area.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 1, 6, Grid_Alignment_Type::Stretch, Device_Section_Row + 1, 1);
        System_Device_Name_Text_Area.Set_Placeholder_Text("Device name");
        System_Device_Name_Text_Area.Set_One_Line(true);

        Log_Verbose("System", "Draw 7");

        // - - - System update button
        System_Update_Button.Create(System_Tab, "Update system", 0, 0, this);
        System_Update_Button.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 0, 4, Grid_Alignment_Type::Stretch, Device_Section_Row + 2, 1);

        Log_Trace();

        // - - - System reboot to loader button
        System_Reboot_Loader_Button.Create(System_Tab, "Reboot to loader", 0, 0, this);
        System_Reboot_Loader_Button.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 4, 4, Grid_Alignment_Type::Stretch, Device_Section_Row + 2, 1);

    }

    // - - Time section

    const uint8_t Time_Section_Row = Device_Section_Row + 3;

    {
        Log_Trace();

        // - - - Time title label

        Label_Type Label;
        Label.Create(System_Tab, "Time");
        Label.Set_Style_Text_Font(&Graphics.Get_Font(24), 0);
        Label.Set_Grid_Cell(Grid_Alignment_Type::Center, 0, 8, Grid_Alignment_Type::Center, Time_Section_Row, 1);
        Label.Clear_Pointer();

        Log_Trace();

        // - - - Apply time button

        System_Time_Apply_Button.Create(System_Tab, "Apply", 0, 0, this);
        System_Time_Apply_Button.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 6, 2, Grid_Alignment_Type::Stretch, Time_Section_Row, 1);

        Log_Trace();

        // - - - NTP server text area

        System_Time_NTP_Server_Text_Area.Create(System_Tab);
        System_Time_NTP_Server_Text_Area.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 1, 6, Grid_Alignment_Type::Stretch, Time_Section_Row + 1, 1);
        System_Time_NTP_Server_Text_Area.Set_Placeholder_Text("N.T.P. server");
        System_Time_NTP_Server_Text_Area.Set_One_Line(true);

        // - - - UTC offset roller

        Log_Trace();

        System_Time_Zone_Roller.Create(System_Tab);
        System_Time_Zone_Roller.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 2, 4, Grid_Alignment_Type::Stretch, Time_Section_Row + 2, 2);
        Static_String_Type<512> Options;
        Static_String_Type<12> Option;

        for (uint8_t i = -12; i <= 12; i++)
        {
            Option.Copy_Format("UTC %s%02u:00\n", i < 0 ? "-" : "+", i < 0 ? -i : i);
            Options += Option;

            if ((i == -4) || (i == -3) || (i == 3) || (i == 4) || (i == 5) || (i == 6) || (i == 9))
            {
                Options.Copy_Format("UTC %s%02u:30\n", i < 0 ? "-" : "+", i < 0 ? -i : i);
                Options += Option;
            }
        }
        System_Time_Zone_Roller.Set_Options(Options, Roller_Mode_Type::Normal);
        System_Time_Zone_Roller.Set_Visible_Row_Count(3);

        Log_Trace();

        // - - - UTC offset label

        Label.Create(System_Tab, "Daylight offset :");
        Label.Set_Grid_Cell(Grid_Alignment_Type::End, 0, 3, Grid_Alignment_Type::Stretch, Time_Section_Row + 3, 1);
        Label.Clear_Pointer();

        Log_Trace();

        // - - - Minus button

        System_Time_Minus_Button.Create(System_Tab, LV_SYMBOL_MINUS, 0, 0, this, Event_Code_Type::All);
        System_Time_Minus_Button.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 3, 1, Grid_Alignment_Type::Stretch, Time_Section_Row + 3, 1);

        Log_Trace();

        // - - - UTC offset spinbox

        System_Time_Daylight_Offset_Spinbox.Create(System_Tab);
        System_Time_Daylight_Offset_Spinbox.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 3, 1, Grid_Alignment_Type::Stretch, Time_Section_Row + 3, 1);
        System_Time_Daylight_Offset_Spinbox.Set_Digit_Format(4, 0);

        Log_Trace();

        // - - - Plus button

        System_Time_Plus_Button.Create(System_Tab, LV_SYMBOL_PLUS, 0, 0, this, Event_Code_Type::All);
        System_Time_Plus_Button.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 3, 1, Grid_Alignment_Type::Stretch, Time_Section_Row + 3, 1);
    }

    Log_Trace();

    // - - About section

    const uint8_t About_Section_Row = Time_Section_Row + 4;

    {
        // - - - About title label

        Label_Type Label;
        Label.Create(System_Tab, "About");
        Label.Set_Style_Text_Font(&Graphics.Get_Font(24), 0);
        Label.Set_Grid_Cell(Grid_Alignment_Type::Center, 0, 8, Grid_Alignment_Type::Center, About_Section_Row, 1);
        Label.Clear_Pointer();

        Log_Trace();

        // - - - Author label

        Label.Create(System_Tab, "Author : Alix ANNERAUD");
        Label.Set_Grid_Cell(Grid_Alignment_Type::Center, 0, 8, Grid_Alignment_Type::Center, About_Section_Row + 1, 1);
        Label.Clear_Pointer();

        Log_Trace();

        // - - - About version

        Label.Create(System_Tab, "Version : " Xila_Version_String);
        Label.Set_Grid_Cell(Grid_Alignment_Type::Center, 0, 8, Grid_Alignment_Type::Center, About_Section_Row + 2, 1);
        Label.Clear_Pointer();

        QRCode_Type QR_Code;
        QR_Code.Create(System_Tab, "https://xila.dev", sizeof("https://xila.dev"), 128);
        QR_Code.Set_Grid_Cell(Grid_Alignment_Type::Center, 0, 8, Grid_Alignment_Type::Center, About_Section_Row + 3, 2);

    }
}