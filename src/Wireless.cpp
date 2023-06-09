/// @file Wireless.cpp
/// @author Alix ANNERAUD (alix@anneraud.fr)
/// @brief
/// @version 0.1.0
/// @date 06-05-2023
///
/// @copyright Copyright (c) 2023

#include "Preferences.hpp"

void Preferences_Class::Refresh_Wireless()
{
    using namespace Xila_Namespace::Communication_Types;

    // - WiFi section

    {
        if (Communication.WiFi.Get_Mode() == Mode_Type::Station)
        {
            Wireless_WiFi_Switch.Add_State(Graphics_Types::State_Type::Checked);
            Wireless_WiFi_Access_Point_Roller.Clear_State(Graphics_Types::State_Type::Disabled);
            Wireless_WiFi_Refresh_Button.Clear_State(Graphics_Types::State_Type::Disabled);
            Wireless_WiFi_Forget_Button.Clear_State(Graphics_Types::State_Type::Disabled);
            Wireless_WiFi_Connect_Button.Clear_State(Graphics_Types::State_Type::Disabled);
            Wireless_WiFi_Password_Text_Area.Clear_State(Graphics_Types::State_Type::Disabled);

            Static_String_Type<32> Connected_SSID;

            if (Communication.WiFi.Station.Get_Status() == Status_Type::Connected)
                Communication.WiFi.Station.Get_SSID(Connected_SSID);

            uint16_t Access_Points_Number = Communication.WiFi.Scan.Start(false, true);

            char Networks_List[33 * (Access_Points_Number) + 12];
            Static_String_Type<32> Temporary_SSID;

            memset(Networks_List, '\0', sizeof(Networks_List));

            for (uint8_t i = 0; i < Access_Points_Number; i++)
            {

                Communication.WiFi.Scan.Get_SSID(i, Temporary_SSID);
                strlcat(Networks_List, Temporary_SSID, sizeof(Networks_List));

                if ((Temporary_SSID == Connected_SSID) && (Connected_SSID != ""))
                    strlcat(Networks_List, " (Connected)", sizeof(Networks_List));

                if (i != (Access_Points_Number - 1))
                    strlcat(Networks_List, "\n", sizeof(Networks_List));
            }

            Wireless_WiFi_Access_Point_Roller.Set_Options(Networks_List, Graphics_Types::Roller_Mode_Type::Normal);
            Communication.WiFi.Scan.Delete();
        }
        else
        {
            Wireless_WiFi_Switch.Clear_State(Graphics_Types::State_Type::Checked);
            Wireless_WiFi_Access_Point_Roller.Add_State(Graphics_Types::State_Type::Disabled);
            Wireless_WiFi_Refresh_Button.Add_State(Graphics_Types::State_Type::Disabled);
            Wireless_WiFi_Forget_Button.Add_State(Graphics_Types::State_Type::Disabled);
            Wireless_WiFi_Connect_Button.Add_State(Graphics_Types::State_Type::Disabled);
            Wireless_WiFi_Password_Text_Area.Add_State(Graphics_Types::State_Type::Disabled);
            Wireless_WiFi_Access_Point_Roller.Set_Options("", Graphics_Types::Roller_Mode_Type::Normal);
        }
    }

    // - Network section

    {
        if (Wireless_Network_DHCP_Checkbox.Has_State(Graphics_Types::State_Type::Checked))
        {
            Wireless_Network_Local_IP_Text_Area.Add_State(Graphics_Types::State_Type::Disabled);
            Wireless_Network_Subnet_Mask_Text_Area.Add_State(Graphics_Types::State_Type::Disabled);
            Wireless_Network_Gateway_IP_Text_Area.Add_State(Graphics_Types::State_Type::Disabled);
            Wireless_Network_DNS_1_Text_Area.Add_State(Graphics_Types::State_Type::Disabled);
            Wireless_Network_DNS_2_Text_Area.Add_State(Graphics_Types::State_Type::Disabled);
        }
        else
        {
            Wireless_Network_Local_IP_Text_Area.Clear_State(Graphics_Types::State_Type::Disabled);
            Wireless_Network_Subnet_Mask_Text_Area.Clear_State(Graphics_Types::State_Type::Disabled);
            Wireless_Network_Gateway_IP_Text_Area.Clear_State(Graphics_Types::State_Type::Disabled);
            Wireless_Network_DNS_1_Text_Area.Clear_State(Graphics_Types::State_Type::Disabled);
            Wireless_Network_DNS_2_Text_Area.Clear_State(Graphics_Types::State_Type::Disabled);
        }

        Static_String_Type<24> IP_Address_String;

        Communication.WiFi.Station.Get_IP_Address().To(IP_Address_String);
        Wireless_Network_Local_IP_Text_Area.Set_Text(IP_Address_String);
        Communication.WiFi.Station.Get_Subnet_Mask().To(IP_Address_String);
        Wireless_Network_Subnet_Mask_Text_Area.Set_Text(IP_Address_String);
        Communication.WiFi.Station.Get_Gateway_IP_Address().To(IP_Address_String);
        Wireless_Network_Gateway_IP_Text_Area.Set_Text(IP_Address_String);
        Communication.WiFi.Station.Get_DNS_IP_Address(0).To(IP_Address_String);
        Wireless_Network_DNS_1_Text_Area.Set_Text(IP_Address_String);
        Communication.WiFi.Station.Get_DNS_IP_Address(1).To(IP_Address_String);
        Wireless_Network_DNS_2_Text_Area.Set_Text(IP_Address_String);
    }
}

void Preferences_Class::Execute_Wireless_Instruction(const Instruction_Type &Instruction)
{
    Graphics_Types::Object_Type Current_Target = Instruction.Graphics.Get_Current_Target();
    if (Current_Target == Wireless_WiFi_Switch)
    {
        if (Wireless_WiFi_Switch.Has_State(Graphics_Types::State_Type::Checked))
            Communication.WiFi.Station.Turn_On();
        else
            Communication.WiFi.Turn_Off();
        Communication.WiFi.Scan.Delete();
        Refresh_Wireless();
    }
    else if (Current_Target == Wireless_WiFi_Refresh_Button)
    {
        Refresh_Wireless();
    }
    else if (Current_Target == Wireless_WiFi_Forget_Button)
    {
        Static_String_Type<32> SSID;

        Communication.WiFi.Station.Remove((const char *)Wireless_WiFi_Access_Point_Roller.Get_Selected_String(SSID));
    }
    else if (Current_Target == Wireless_WiFi_Connect_Button)
    {
        Static_String_Type<32> SSID;

        if (strcmp(Wireless_WiFi_Password_Text_Area.Get_Text(), "") != 0)
            Communication.WiFi.Station.Remove((const char *)Wireless_WiFi_Access_Point_Roller.Get_Selected_String(SSID));

        Main_Task.Delay(10);

        Communication.WiFi.Station.Connect((const char *)Wireless_WiFi_Access_Point_Roller.Get_Selected_String(SSID), (const char *)Wireless_WiFi_Password_Text_Area.Get_Text());
    }
    else if (Current_Target == Wireless_Network_DHCP_Checkbox)
    {
        Refresh_Wireless();
    }
}

void Preferences_Class::Draw_Wireless()
{
    using namespace Graphics_Types;

    Network_Tab.Set_Flex_Flow(Flex_Flow_Type::Row);
    Network_Tab.Set_Style_Pad_All(0, 0);
    Network_Tab.Clear_Flag(Flag_Type::Scroll_Elastic);
    Network_Tab.Clear_Flag(Flag_Type::Scroll_Momentum);

    // - Grid layout
    Object_Type Grid;
    Grid.Create(Network_Tab);
    Grid.Set_Size(Percentage(100), Percentage(100));
    Grid.Set_Style_Background_Opacity(Opacity_Type::Transparent, 0);
    Grid.Set_Grid_Descriptor_Array(Column_Descriptor, Row_Descriptor);
    Grid.Set_Style_Pad_All(10, 0);

    const uint8_t WiFi_Section_Row = 0;

    {
        Label_Type Label;
        Label.Create(Grid, "WiFi station");
        Label.Set_Style_Text_Font(&Graphics.Get_Font(24), 0);
        Label.Set_Grid_Cell(Grid_Alignment_Type::Center, 0, 8, Grid_Alignment_Type::Center, WiFi_Section_Row, 1);
        Label.Clear_Pointer();

        // - WiFi switch
        Wireless_WiFi_Switch.Create(Grid);
        Wireless_WiFi_Switch.Set_Grid_Cell(Grid_Alignment_Type::Center, 6, 2, Grid_Alignment_Type::Center, WiFi_Section_Row, 1);
        Wireless_WiFi_Switch.Add_Event(this, Event_Code_Type::Value_Changed);

        // - Access point roller
        Wireless_WiFi_Access_Point_Roller.Create(Grid);
        Wireless_WiFi_Access_Point_Roller.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 0, 6, Grid_Alignment_Type::Stretch, WiFi_Section_Row + 1, 3);

        // - Refresh button

        Wireless_WiFi_Refresh_Button.Create(Grid, "Refresh", 0, 0, this);
        Wireless_WiFi_Refresh_Button.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 6, 2, Grid_Alignment_Type::Stretch, WiFi_Section_Row + 1, 1);

        // - Informations button

        Wireless_WiFi_Forget_Button.Create(Grid, "Forget", 0, 0, this);
        Wireless_WiFi_Forget_Button.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 6, 2, Grid_Alignment_Type::Stretch, WiFi_Section_Row + 2, 1);

        // - Connect button

        Wireless_WiFi_Connect_Button.Create(Grid, "Connect", 0, 0, this);
        Wireless_WiFi_Connect_Button.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 6, 2, Grid_Alignment_Type::Stretch, WiFi_Section_Row + 3, 1);

        // - Password text area

        Wireless_WiFi_Password_Text_Area.Create(Grid);
        Wireless_WiFi_Password_Text_Area.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 0, 6, Grid_Alignment_Type::Stretch, WiFi_Section_Row + 4, 1);
        Wireless_WiFi_Password_Text_Area.Set_Password_Mode(true);
        Wireless_WiFi_Password_Text_Area.Set_Placeholder_Text("Password");
        Wireless_WiFi_Password_Text_Area.Set_One_Line(true);
        Wireless_WiFi_Password_Text_Area.Add_Event(this, Event_Code_Type::Focused);
        Wireless_WiFi_Password_Text_Area.Add_Event(this, Event_Code_Type::Defocused);
    }

    const uint8_t Network_Section_Row = WiFi_Section_Row + 5;

    {
        // - Title label

        Label_Type Label;
        Label.Create(Grid, "Network");
        Label.Set_Style_Text_Font(&Graphics.Get_Font(24), 0);
        Label.Set_Grid_Cell(Grid_Alignment_Type::Center, 0, 8, Grid_Alignment_Type::Center, Network_Section_Row, 1);
        Label.Clear_Pointer();

        // - Apply button

        Wireless_Network_Apply_Button.Create(Grid, "Apply", 0, 0, this);
        Wireless_Network_Apply_Button.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 6, 2, Grid_Alignment_Type::Stretch, Network_Section_Row, 1);

        // - Local IP
        Wireless_Network_Local_IP_Text_Area.Create(Grid);
        Wireless_Network_Local_IP_Text_Area.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 0, 6, Grid_Alignment_Type::Stretch, Network_Section_Row + 1, 1);
        Wireless_Network_Local_IP_Text_Area.Set_Placeholder_Text("Local IP");
        Wireless_Network_Local_IP_Text_Area.Set_One_Line(true);
        Wireless_Network_Local_IP_Text_Area.Set_Maximum_Length(15);
        Wireless_Network_Local_IP_Text_Area.Set_Accepted_Characters("0123456789.");
        Wireless_Network_Local_IP_Text_Area.Add_Event(this, Event_Code_Type::Focused);
        Wireless_Network_Local_IP_Text_Area.Add_Event(this, Event_Code_Type::Defocused);

        // - DHCP checkbox
        Wireless_Network_DHCP_Checkbox.Create(Grid);
        Wireless_Network_DHCP_Checkbox.Set_Grid_Cell(Grid_Alignment_Type::Center, 6, 2, Grid_Alignment_Type::Center, Network_Section_Row + 1, 1);
        Wireless_Network_DHCP_Checkbox.Set_Text("DHCP");
        Wireless_Network_DHCP_Checkbox.Add_Event(this, Event_Code_Type::Value_Changed);

        // - Subnet mask
        Wireless_Network_Subnet_Mask_Text_Area.Create(Grid);
        Wireless_Network_Subnet_Mask_Text_Area.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 0, 6, Grid_Alignment_Type::Stretch, Network_Section_Row + 2, 1);
        Wireless_Network_Subnet_Mask_Text_Area.Set_Placeholder_Text("Subnet mask");
        Wireless_Network_Subnet_Mask_Text_Area.Set_One_Line(true);
        Wireless_Network_Subnet_Mask_Text_Area.Set_Maximum_Length(15);
        Wireless_Network_Subnet_Mask_Text_Area.Set_Accepted_Characters("0123456789.");
        Wireless_Network_Subnet_Mask_Text_Area.Add_Event(this, Event_Code_Type::Focused);
        Wireless_Network_Subnet_Mask_Text_Area.Add_Event(this, Event_Code_Type::Defocused);

        // - Gateway
        Wireless_Network_Gateway_IP_Text_Area.Create(Grid);
        Wireless_Network_Gateway_IP_Text_Area.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 0, 6, Grid_Alignment_Type::Stretch, Network_Section_Row + 3, 1);
        Wireless_Network_Gateway_IP_Text_Area.Set_Placeholder_Text("Gateway");
        Wireless_Network_Gateway_IP_Text_Area.Set_One_Line(true);
        Wireless_Network_Gateway_IP_Text_Area.Set_Maximum_Length(15);
        Wireless_Network_Gateway_IP_Text_Area.Set_Accepted_Characters("0123456789.");
        Wireless_Network_Gateway_IP_Text_Area.Add_Event(this, Event_Code_Type::Focused);
        Wireless_Network_Gateway_IP_Text_Area.Add_Event(this, Event_Code_Type::Defocused);

        // - DNS 1
        Wireless_Network_DNS_1_Text_Area.Create(Grid);
        Wireless_Network_DNS_1_Text_Area.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 0, 6, Grid_Alignment_Type::Stretch, Network_Section_Row + 4, 1);
        Wireless_Network_DNS_1_Text_Area.Set_Placeholder_Text("DNS 1");
        Wireless_Network_DNS_1_Text_Area.Set_One_Line(true);
        Wireless_Network_DNS_1_Text_Area.Set_Maximum_Length(15);
        Wireless_Network_DNS_1_Text_Area.Set_Accepted_Characters("0123456789.");
        Wireless_Network_DNS_1_Text_Area.Add_Event(this, Event_Code_Type::Focused);
        Wireless_Network_DNS_1_Text_Area.Add_Event(this, Event_Code_Type::Defocused);

        // - DNS 2
        Wireless_Network_DNS_2_Text_Area.Create(Grid);
        Wireless_Network_DNS_2_Text_Area.Set_Grid_Cell(Grid_Alignment_Type::Stretch, 0, 6, Grid_Alignment_Type::Stretch, Network_Section_Row + 5, 1);
        Wireless_Network_DNS_2_Text_Area.Set_Placeholder_Text("DNS 2");
        Wireless_Network_DNS_2_Text_Area.Set_One_Line(true);
        Wireless_Network_DNS_2_Text_Area.Set_Maximum_Length(15);
        Wireless_Network_DNS_2_Text_Area.Set_Accepted_Characters("0123456789.");
        Wireless_Network_DNS_2_Text_Area.Add_Event(this, Event_Code_Type::Focused);
        Wireless_Network_DNS_2_Text_Area.Add_Event(this, Event_Code_Type::Defocused);
    }
}