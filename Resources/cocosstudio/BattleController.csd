<GameProjectFile>
  <PropertyGroup Type="Layer" Name="BattleController" ID="0bd9e65c-114c-49fa-80b7-5fd467b07ea8" Version="2.0.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="0" Speed="1.0000" />
      <ObjectData Name="Layer" CanEdit="False" FrameEvent="" ComboBoxIndex="1" ColorAngle="0.0000" ctype="PanelObjectData">
        <Position X="0.0000" Y="0.0000" />
        <Scale ScaleX="1.0000" ScaleY="1.0000" />
        <AnchorPoint />
        <CColor A="255" R="255" G="255" B="255" />
        <Size X="960.0000" Y="640.0000" />
        <PrePosition X="0.0000" Y="0.0000" />
        <PreSize X="0.0000" Y="0.0000" />
        <Children>
          <NodeObjectData Name="UnitSelectorPanel" ActionTag="6" FrameEvent="" Tag="4" ObjectIndex="1" TouchEnable="True" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
            <Position X="10.0000" Y="10.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <AnchorPoint />
            <CColor A="255" R="255" G="255" B="255" />
            <Size X="940.0000" Y="140.0000" />
            <PrePosition X="0.0104" Y="0.0156" />
            <PreSize X="0.9792" Y="0.2188" />
            <Children>
              <NodeObjectData Name="Panel_Barbarian" ActionTag="7" FrameEvent="" Tag="5" ObjectIndex="2" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                <Position X="34.0000" Y="15.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="83.0000" Y="110.0000" />
                <PrePosition X="0.0362" Y="0.1071" />
                <PreSize X="0.0883" Y="0.7857" />
                <Children>
                  <NodeObjectData Name="stage_ui_unit_background_Copy_Copy_Copy_Copy" ActionTag="-1258676611" FrameEvent="" Tag="94" ObjectIndex="31" ctype="SpriteObjectData">
                    <Position X="0.0000" Y="0.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="86.0000" Y="108.0000" />
                    <PrePosition X="0.0000" Y="0.0000" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/unit_background.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="Sprite_4" ActionTag="11" FrameEvent="" Tag="9" ObjectIndex="4" ctype="SpriteObjectData">
                    <Position X="43.0001" Y="45.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="83.0000" Y="83.0000" />
                    <PrePosition X="0.5181" Y="0.4091" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/BarbarianIcon.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="stage_ui_unit_frame_Barbarian" ActionTag="952744688" VisibleForFrame="False" FrameEvent="" Tag="95" ObjectIndex="32" ctype="SpriteObjectData">
                    <Position X="-2.9991" Y="-4.6665" />
                    <Scale ScaleX="1.0500" ScaleY="1.0500" />
                    <AnchorPoint />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="86.0000" Y="109.0000" />
                    <PrePosition X="-0.0361" Y="-0.0424" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/unit_frame.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="Text_UnitCounter_Barbarian" ActionTag="32" FrameEvent="" Tag="30" ObjectIndex="1" FontSize="22" LabelText="x99&#xA;" ctype="TextObjectData">
                    <Position X="38.0000" Y="96.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="41.0000" Y="24.0000" />
                    <PrePosition X="0.4578" Y="0.8727" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FontResource Type="Normal" Path="fonts/CCBackBeat-Light_5.ttf" />
                  </NodeObjectData>
                  <NodeObjectData Name="stage_ui_unit_black" ActionTag="1822495324" Rotation="90.0000" RotationSkewX="90.0000" RotationSkewY="90.0000" Alpha="235" VisibleForFrame="False" FrameEvent="" Tag="87" ObjectIndex="42" ctype="SpriteObjectData">
                    <Position X="43.0009" Y="53.5000" />
                    <Scale ScaleX="2.4000" ScaleY="2.6400" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="48.0000" Y="36.0000" />
                    <PrePosition X="0.5181" Y="0.4864" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/unit_black.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="Button_Barbarian" CanEdit="False" ActionTag="-1132376506" Alpha="0" FrameEvent="" Tag="59" ObjectIndex="8" TouchEnable="True" FontSize="14" ButtonText="" Scale9Enable="True" Scale9Width="46" Scale9Height="36" ctype="ButtonObjectData">
                    <Position X="0.0000" Y="0.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="83.0000" Y="110.0000" />
                    <PrePosition X="0.0000" Y="0.0000" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" />
                    <NormalFileData Type="Default" Path="Default/Button_Normal.png" />
                  </NodeObjectData>
                </Children>
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </NodeObjectData>
              <NodeObjectData Name="Panel_Archer" ActionTag="13" FrameEvent="" Tag="11" ObjectIndex="3" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                <Position X="137.7500" Y="15.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="83.0000" Y="110.0000" />
                <PrePosition X="0.1465" Y="0.1071" />
                <PreSize X="0.0883" Y="0.7857" />
                <Children>
                  <NodeObjectData Name="stage_ui_unit_background_Copy_Copy_Copy" ActionTag="-733109858" FrameEvent="" Tag="92" ObjectIndex="29" ctype="SpriteObjectData">
                    <Position X="0.0000" Y="0.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="86.0000" Y="108.0000" />
                    <PrePosition X="0.0000" Y="0.0000" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/unit_background.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="Sprite_5" ActionTag="12" FrameEvent="" Tag="10" ObjectIndex="5" ctype="SpriteObjectData">
                    <Position X="41.0001" Y="46.0001" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="74.0000" Y="83.0000" />
                    <PrePosition X="0.4940" Y="0.4182" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/ArcherIcon.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="stage_ui_unit_frame_Archer" ActionTag="1269844411" VisibleForFrame="False" FrameEvent="" Tag="93" ObjectIndex="30" ctype="SpriteObjectData">
                    <Position X="-1.6659" Y="-3.3333" />
                    <Scale ScaleX="1.0500" ScaleY="1.0500" />
                    <AnchorPoint />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="86.0000" Y="109.0000" />
                    <PrePosition X="-0.0201" Y="-0.0303" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/unit_frame.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="Text_UnitCounter_Archer" ActionTag="33" FrameEvent="" Tag="31" ObjectIndex="2" FontSize="22" LabelText="x10" ctype="TextObjectData">
                    <Position X="41.0000" Y="96.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="34.0000" Y="24.0000" />
                    <PrePosition X="0.4940" Y="0.8727" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FontResource Type="Normal" Path="fonts/CCBackBeat-Light_5.ttf" />
                  </NodeObjectData>
                  <NodeObjectData Name="stage_ui_unit_black" ActionTag="1101650737" Rotation="90.0000" RotationSkewX="90.0000" RotationSkewY="90.0000" Alpha="235" VisibleForFrame="False" FrameEvent="" Tag="86" ObjectIndex="41" ctype="SpriteObjectData">
                    <Position X="43.0007" Y="53.5000" />
                    <Scale ScaleX="2.4000" ScaleY="2.6400" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="48.0000" Y="36.0000" />
                    <PrePosition X="0.5181" Y="0.4864" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/unit_black.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="Button_Archer" CanEdit="False" ActionTag="-956704879" Alpha="0" FrameEvent="" Tag="58" ObjectIndex="7" TouchEnable="True" FontSize="14" ButtonText="" Scale9Enable="True" Scale9Width="46" Scale9Height="36" ctype="ButtonObjectData">
                    <Position X="0.0000" Y="0.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="83.0000" Y="110.0000" />
                    <PrePosition X="0.0000" Y="0.0000" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" />
                    <NormalFileData Type="Default" Path="Default/Button_Normal.png" />
                  </NodeObjectData>
                </Children>
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </NodeObjectData>
              <NodeObjectData Name="Panel_Goblin" ActionTag="15" FrameEvent="" Tag="13" ObjectIndex="4" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                <Position X="240.5000" Y="14.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="83.0000" Y="110.0000" />
                <PrePosition X="0.2559" Y="0.1000" />
                <PreSize X="0.0883" Y="0.7857" />
                <Children>
                  <NodeObjectData Name="stage_ui_unit_background_Copy_Copy" ActionTag="170333472" FrameEvent="" Tag="91" ObjectIndex="28" ctype="SpriteObjectData">
                    <Position X="0.0000" Y="0.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="86.0000" Y="108.0000" />
                    <PrePosition X="0.0000" Y="0.0000" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/unit_background.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="Sprite_2" ActionTag="9" FrameEvent="" Tag="7" ObjectIndex="2" ctype="SpriteObjectData">
                    <Position X="45.6670" Y="43.8335" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="83.0000" Y="81.0000" />
                    <PrePosition X="0.5502" Y="0.3985" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/GoblinIcon.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="stage_ui_unit_frame_Goblin" ActionTag="65227225" VisibleForFrame="False" FrameEvent="" Tag="90" ObjectIndex="27" ctype="SpriteObjectData">
                    <Position X="-1.9996" Y="-3.0000" />
                    <Scale ScaleX="1.0500" ScaleY="1.0500" />
                    <AnchorPoint />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="86.0000" Y="109.0000" />
                    <PrePosition X="-0.0241" Y="-0.0273" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/unit_frame.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="Text_UnitCounter_Goblin" ActionTag="34" FrameEvent="" Tag="32" ObjectIndex="3" FontSize="22" LabelText="x10" ctype="TextObjectData">
                    <Position X="41.0000" Y="96.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="34.0000" Y="24.0000" />
                    <PrePosition X="0.4940" Y="0.8727" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FontResource Type="Normal" Path="fonts/CCBackBeat-Light_5.ttf" />
                  </NodeObjectData>
                  <NodeObjectData Name="stage_ui_unit_black" ActionTag="-62026433" Rotation="90.0000" RotationSkewX="90.0000" RotationSkewY="90.0000" Alpha="235" VisibleForFrame="False" FrameEvent="" Tag="85" ObjectIndex="40" ctype="SpriteObjectData">
                    <Position X="43.0007" Y="53.5000" />
                    <Scale ScaleX="2.4000" ScaleY="2.6400" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="48.0000" Y="36.0000" />
                    <PrePosition X="0.5181" Y="0.4864" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/unit_black.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="Button_Goblin" CanEdit="False" ActionTag="-1701015806" Alpha="0" FrameEvent="" Tag="57" ObjectIndex="6" TouchEnable="True" FontSize="14" ButtonText="" Scale9Enable="True" Scale9Width="46" Scale9Height="36" ctype="ButtonObjectData">
                    <Position X="0.0000" Y="0.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="83.0000" Y="110.0000" />
                    <PrePosition X="0.0000" Y="0.0000" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" />
                    <NormalFileData Type="Default" Path="Default/Button_Normal.png" />
                  </NodeObjectData>
                </Children>
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </NodeObjectData>
              <NodeObjectData Name="Panel_Giant" ActionTag="17" FrameEvent="" Tag="15" ObjectIndex="5" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                <Position X="342.2500" Y="13.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="83.0000" Y="110.0000" />
                <PrePosition X="0.3641" Y="0.0929" />
                <PreSize X="0.0883" Y="0.7857" />
                <Children>
                  <NodeObjectData Name="stage_ui_unit_background_Copy" ActionTag="-1959632025" FrameEvent="" Tag="87" ObjectIndex="24" ctype="SpriteObjectData">
                    <Position X="0.0000" Y="0.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="86.0000" Y="108.0000" />
                    <PrePosition X="0.0000" Y="0.0000" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/unit_background.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="Sprite_3" ActionTag="10" FrameEvent="" Tag="8" ObjectIndex="3" ctype="SpriteObjectData">
                    <Position X="42.5002" Y="41.0001" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="83.0000" Y="83.0000" />
                    <PrePosition X="0.5121" Y="0.3727" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/GiantIcon.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="stage_ui_unit_frame_Giant" ActionTag="-1411730493" VisibleForFrame="False" FrameEvent="" Tag="89" ObjectIndex="26" ctype="SpriteObjectData">
                    <Position X="-1.9999" Y="-2.0000" />
                    <Scale ScaleX="1.0500" ScaleY="1.0500" />
                    <AnchorPoint />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="86.0000" Y="109.0000" />
                    <PrePosition X="-0.0241" Y="-0.0182" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/unit_frame.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="Text_UnitCounter_Giant" ActionTag="35" FrameEvent="" Tag="33" ObjectIndex="4" FontSize="22" LabelText="x10" ctype="TextObjectData">
                    <Position X="41.0000" Y="96.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="34.0000" Y="24.0000" />
                    <PrePosition X="0.4940" Y="0.8727" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FontResource Type="Normal" Path="fonts/CCBackBeat-Light_5.ttf" />
                  </NodeObjectData>
                  <NodeObjectData Name="stage_ui_unit_black" ActionTag="1489013445" Rotation="90.0000" RotationSkewX="90.0000" RotationSkewY="90.0000" Alpha="235" VisibleForFrame="False" FrameEvent="" Tag="83" ObjectIndex="38" ctype="SpriteObjectData">
                    <Position X="43.0004" Y="53.5000" />
                    <Scale ScaleX="2.4000" ScaleY="2.6400" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="48.0000" Y="36.0000" />
                    <PrePosition X="0.5181" Y="0.4864" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/unit_black.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="Button_Giant" CanEdit="False" ActionTag="-1825358047" Alpha="0" FrameEvent="" Tag="56" ObjectIndex="5" TouchEnable="True" FontSize="14" ButtonText="" Scale9Enable="True" Scale9Width="46" Scale9Height="36" ctype="ButtonObjectData">
                    <Position X="0.0000" Y="0.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="83.0000" Y="110.0000" />
                    <PrePosition X="0.0000" Y="0.0000" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" />
                    <NormalFileData Type="Default" Path="Default/Button_Normal.png" />
                  </NodeObjectData>
                </Children>
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </NodeObjectData>
              <NodeObjectData Name="Panel_Wallbreaker" CanEdit="False" ActionTag="19" FrameEvent="" Tag="17" ObjectIndex="6" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                <Position X="440.0000" Y="12.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="83.0000" Y="110.0000" />
                <PrePosition X="0.4681" Y="0.0857" />
                <PreSize X="0.0883" Y="0.7857" />
                <Children>
                  <NodeObjectData Name="stage_ui_unit_background" ActionTag="-1732595739" FrameEvent="" Tag="85" ObjectIndex="22" ctype="SpriteObjectData">
                    <Position X="0.0000" Y="0.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="86.0000" Y="108.0000" />
                    <PrePosition X="0.0000" Y="0.0000" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/unit_background.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="Sprite_1_Copy_Copy" ActionTag="20" FrameEvent="" Tag="18" ObjectIndex="9" ctype="SpriteObjectData">
                    <Position X="41.0001" Y="46.0001" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="83.0000" Y="83.0000" />
                    <PrePosition X="0.4940" Y="0.4182" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/WallbreakerIcon.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="stage_ui_unit_frame_Wallbreaker" ActionTag="-2029377484" VisibleForFrame="False" FrameEvent="" Tag="86" ObjectIndex="23" ctype="SpriteObjectData">
                    <Position X="-0.9998" Y="-3.0000" />
                    <Scale ScaleX="1.0500" ScaleY="1.0500" />
                    <AnchorPoint />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="86.0000" Y="109.0000" />
                    <PrePosition X="-0.0120" Y="-0.0273" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/unit_frame.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="Text_UnitCounter_Wallbreaker" ActionTag="37" FrameEvent="" Tag="35" ObjectIndex="6" FontSize="22" LabelText="x10" ctype="TextObjectData">
                    <Position X="40.0000" Y="95.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="34.0000" Y="24.0000" />
                    <PrePosition X="0.4819" Y="0.8636" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FontResource Type="Normal" Path="fonts/CCBackBeat-Light_5.ttf" />
                  </NodeObjectData>
                  <NodeObjectData Name="stage_ui_unit_black" ActionTag="1657591544" Rotation="90.0000" RotationSkewX="90.0000" RotationSkewY="90.0000" Alpha="235" VisibleForFrame="False" FrameEvent="" Tag="81" ObjectIndex="36" ctype="SpriteObjectData">
                    <Position X="43.0002" Y="53.5000" />
                    <Scale ScaleX="2.4000" ScaleY="2.6400" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="48.0000" Y="36.0000" />
                    <PrePosition X="0.5181" Y="0.4864" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/ui/unit_black.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="Button_Wallbreaker" ActionTag="1745189163" Alpha="0" FrameEvent="" Tag="55" ObjectIndex="4" TouchEnable="True" FontSize="14" ButtonText="" Scale9Enable="True" Scale9Width="46" Scale9Height="36" ctype="ButtonObjectData">
                    <Position X="0.0000" Y="0.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="83.0000" Y="110.0000" />
                    <PrePosition X="0.0000" Y="0.0000" />
                    <PreSize X="1.0000" Y="1.0000" />
                    <TextColor A="255" R="65" G="65" B="70" />
                    <DisabledFileData Type="Default" Path="Default/Button_Disable.png" />
                    <PressedFileData Type="Default" Path="Default/Button_Press.png" />
                    <NormalFileData Type="Default" Path="Default/Button_Normal.png" />
                  </NodeObjectData>
                </Children>
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </NodeObjectData>
              <NodeObjectData Name="stage_ui_Empty_Unit" ActionTag="187879826" FrameEvent="" Tag="81" ObjectIndex="18" ctype="SpriteObjectData">
                <Position X="577.0000" Y="67.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="86.0000" Y="109.0000" />
                <PrePosition X="0.6138" Y="0.4786" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="stage/ui/Empty_Unit.png" Plist="assets.plist" />
              </NodeObjectData>
              <NodeObjectData Name="stage_ui_Empty_Unit_Copy" ActionTag="1437833570" FrameEvent="" Tag="82" ObjectIndex="19" ctype="SpriteObjectData">
                <Position X="672.0000" Y="67.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="86.0000" Y="109.0000" />
                <PrePosition X="0.7149" Y="0.4786" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="stage/ui/Empty_Unit.png" Plist="assets.plist" />
              </NodeObjectData>
              <NodeObjectData Name="stage_ui_Empty_Unit_Copy_Copy" ActionTag="1013554461" FrameEvent="" Tag="83" ObjectIndex="20" ctype="SpriteObjectData">
                <Position X="769.0000" Y="67.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="86.0000" Y="109.0000" />
                <PrePosition X="0.8181" Y="0.4786" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="stage/ui/Empty_Unit.png" Plist="assets.plist" />
              </NodeObjectData>
              <NodeObjectData Name="stage_ui_Empty_Unit_Copy_Copy_Copy" ActionTag="-1625305115" FrameEvent="" Tag="84" ObjectIndex="21" ctype="SpriteObjectData">
                <Position X="865.0000" Y="67.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="86.0000" Y="109.0000" />
                <PrePosition X="0.9202" Y="0.4786" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="stage/ui/Empty_Unit.png" Plist="assets.plist" />
              </NodeObjectData>
            </Children>
            <SingleColor A="255" R="37" G="37" B="37" />
            <FirstColor A="255" R="37" G="37" B="37" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </NodeObjectData>
          <NodeObjectData Name="EndBattleButtonPanel" ActionTag="28" FrameEvent="" Tag="26" ObjectIndex="7" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
            <Position X="12.5875" Y="155.2104" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <AnchorPoint />
            <CColor A="255" R="255" G="255" B="255" />
            <Size X="148.0000" Y="40.0000" />
            <PrePosition X="0.0131" Y="0.2425" />
            <PreSize X="0.1542" Y="0.0625" />
            <Children>
              <NodeObjectData Name="Image_RedButton" ActionTag="-1772562742" FrameEvent="" Tag="63" ObjectIndex="2" LeftEage="12" RightEage="12" Scale9OriginX="12" Scale9Width="128" Scale9Height="38" ctype="ImageViewObjectData">
                <Position X="0.0000" Y="0.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="152.0000" Y="38.0000" />
                <PrePosition X="0.0000" Y="0.0000" />
                <PreSize X="1.0000" Y="0.8000" />
                <FileData Type="PlistSubImage" Path="stage/ui/RedButton.png" Plist="assets.plist" />
              </NodeObjectData>
              <NodeObjectData Name="Text_EndBattle_Shadow" ActionTag="1090316623" FrameEvent="" Tag="52" ObjectIndex="31" FontSize="13" LabelText="End Battle" ctype="TextObjectData">
                <Position X="21.1254" Y="11.6555" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="25" G="25" B="25" />
                <Size X="107.0000" Y="14.0000" />
                <PrePosition X="0.1427" Y="0.2914" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="Text_7" ActionTag="38" FrameEvent="" Tag="36" ObjectIndex="7" FontSize="13" LabelText="End Battle" ctype="TextObjectData">
                <Position X="19.6523" Y="12.9977" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="107.0000" Y="14.0000" />
                <PrePosition X="0.1328" Y="0.3249" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="Button_EndButtle" ActionTag="74" Alpha="0" FrameEvent="" Tag="72" ObjectIndex="1" TouchEnable="True" FontSize="14" ButtonText="" Scale9Enable="True" Scale9Width="46" Scale9Height="36" ctype="ButtonObjectData">
                <Position X="0.0000" Y="0.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="148.0000" Y="36.0000" />
                <PrePosition X="0.0000" Y="0.0000" />
                <PreSize X="1.0000" Y="0.9000" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" />
                <PressedFileData Type="Default" Path="Default/Button_Press.png" />
                <NormalFileData Type="Default" Path="Default/Button_Normal.png" />
              </NodeObjectData>
            </Children>
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </NodeObjectData>
          <NodeObjectData Name="AvailableLootPanel" ActionTag="39" FrameEvent="" Tag="37" ObjectIndex="8" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
            <Position X="13.5000" Y="492.5000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <AnchorPoint />
            <CColor A="255" R="255" G="255" B="255" />
            <Size X="160.0000" Y="100.0000" />
            <PrePosition X="0.0141" Y="0.7695" />
            <PreSize X="0.1667" Y="0.1563" />
            <Children>
              <NodeObjectData Name="Sprite_14" ActionTag="41" FrameEvent="" Tag="39" ObjectIndex="14" ctype="SpriteObjectData">
                <Position X="15.9999" Y="51.9994" />
                <Scale ScaleX="0.7500" ScaleY="0.7500" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="36.0000" Y="37.0000" />
                <PrePosition X="0.1000" Y="0.5200" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="stage/ui/gold.png" Plist="assets.plist" />
              </NodeObjectData>
              <NodeObjectData Name="Sprite_15" ActionTag="42" FrameEvent="" Tag="40" ObjectIndex="15" ctype="SpriteObjectData">
                <Position X="14.9999" Y="19.9999" />
                <Scale ScaleX="0.7500" ScaleY="0.7500" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="33.0000" Y="38.0000" />
                <PrePosition X="0.0937" Y="0.2000" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="stage/ui/elixer.png" Plist="assets.plist" />
              </NodeObjectData>
              <NodeObjectData Name="Text_Coin_Shadow" ActionTag="-1136972161" FrameEvent="" Tag="47" ObjectIndex="26" FontSize="20" LabelText="10 000" ctype="TextObjectData">
                <Position X="84.0000" Y="50.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="20" G="20" B="20" />
                <Size X="99.0000" Y="22.0000" />
                <PrePosition X="0.5250" Y="0.5000" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="Text_Coin" ActionTag="43" FrameEvent="" Tag="41" ObjectIndex="8" FontSize="20" LabelText="10 000" ctype="TextObjectData">
                <Position X="82.0000" Y="52.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="99.0000" Y="22.0000" />
                <PrePosition X="0.4100" Y="0.5200" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="Text_Elixer_Shadow" ActionTag="2118112792" FrameEvent="" Tag="48" ObjectIndex="27" FontSize="20" LabelText="10 000" ctype="TextObjectData">
                <Position X="84.0000" Y="18.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="28" G="28" B="28" />
                <Size X="99.0000" Y="22.0000" />
                <PrePosition X="0.5250" Y="0.1800" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="Text_Elixer" ActionTag="44" FrameEvent="" Tag="42" ObjectIndex="9" FontSize="20" LabelText="10 000" ctype="TextObjectData">
                <Position X="82.0000" Y="20.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="99.0000" Y="22.0000" />
                <PrePosition X="0.4100" Y="0.2000" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="Text_8_Copy_0_Shade" ActionTag="-2090867919" FrameEvent="" Tag="46" ObjectIndex="25" FontSize="10" LabelText="Available Loot:&#xA;" ctype="TextObjectData">
                <Position X="86.0000" Y="84.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="24" G="24" B="24" />
                <Size X="110.0000" Y="11.0000" />
                <PrePosition X="0.5375" Y="0.8400" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="Text_8_Copy_0" ActionTag="45" FrameEvent="" Tag="43" ObjectIndex="10" FontSize="10" LabelText="Available Loot:&#xA;" ctype="TextObjectData">
                <Position X="84.0000" Y="86.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="110.0000" Y="11.0000" />
                <PrePosition X="0.4200" Y="0.8600" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
            </Children>
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </NodeObjectData>
          <NodeObjectData Name="FieldNamePanel" ActionTag="46" FrameEvent="" Tag="44" ObjectIndex="9" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
            <Position X="29.5000" Y="607.5000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <AnchorPoint />
            <CColor A="255" R="255" G="255" B="255" />
            <Size X="180.0000" Y="20.0000" />
            <PrePosition X="0.0307" Y="0.9492" />
            <PreSize X="0.1875" Y="0.0313" />
            <Children>
              <NodeObjectData Name="Text_11_Shade" ActionTag="1286676089" FrameEvent="" Tag="45" ObjectIndex="24" FontSize="18" LabelText="Ommahha Beech" ctype="TextObjectData">
                <Position X="-2.0000" Y="0.9974" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="24" G="24" B="24" />
                <Size X="204.0000" Y="20.0000" />
                <PrePosition X="-0.0111" Y="0.0499" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="Text_11" ActionTag="47" FrameEvent="" Tag="45" ObjectIndex="11" FontSize="18" LabelText="Ommahha Beech" ctype="TextObjectData">
                <Position X="-4.0000" Y="2.9995" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="204.0000" Y="20.0000" />
                <PrePosition X="-0.0222" Y="0.1500" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
            </Children>
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </NodeObjectData>
          <NodeObjectData Name="BattleRemainLabelPanel" ActionTag="48" FrameEvent="" Tag="46" ObjectIndex="10" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
            <Position X="377.5000" Y="601.5000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <AnchorPoint />
            <CColor A="255" R="255" G="255" B="255" />
            <Size X="160.0000" Y="28.0000" />
            <PrePosition X="0.3932" Y="0.9398" />
            <PreSize X="0.1667" Y="0.0437" />
            <Children>
              <NodeObjectData Name="Text_Timer_Shadow" ActionTag="-1335267161" FrameEvent="" Tag="49" ObjectIndex="28" FontSize="22" LabelText="2 m 32 s" HorizontalAlignmentType="HT_Center" ctype="TextObjectData">
                <Position X="3.0000" Y="0.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="30" G="30" B="30" />
                <Size X="119.0000" Y="24.0000" />
                <PrePosition X="0.0250" Y="0.0000" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="Text_Timer" ActionTag="49" FrameEvent="" Tag="47" ObjectIndex="12" FontSize="22" LabelText="2 m 32 s" HorizontalAlignmentType="HT_Center" ctype="TextObjectData">
                <Position X="0.0000" Y="2.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="119.0000" Y="24.0000" />
                <PrePosition X="0.0000" Y="0.0714" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
            </Children>
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </NodeObjectData>
          <NodeObjectData Name="PlayersAssetPanel" ActionTag="50" FrameEvent="" Tag="48" ObjectIndex="11" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
            <Position X="792.5000" Y="512.5000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <AnchorPoint />
            <CColor A="255" R="255" G="255" B="255" />
            <Size X="160.0000" Y="120.0000" />
            <PrePosition X="0.8255" Y="0.8008" />
            <PreSize X="0.1667" Y="0.1875" />
            <Children>
              <NodeObjectData Name="Sprite_14_Copy" ActionTag="51" FrameEvent="" Tag="49" ObjectIndex="16" ctype="SpriteObjectData">
                <Position X="59.0000" Y="87.9999" />
                <Scale ScaleX="0.7500" ScaleY="0.7500" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="36.0000" Y="37.0000" />
                <PrePosition X="0.3688" Y="0.7333" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="stage/ui/gold.png" Plist="assets.plist" />
              </NodeObjectData>
              <NodeObjectData Name="Sprite_15_Copy" ActionTag="52" FrameEvent="" Tag="50" ObjectIndex="17" ctype="SpriteObjectData">
                <Position X="57.0002" Y="33.0000" />
                <Scale ScaleX="0.7500" ScaleY="0.7500" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="33.0000" Y="38.0000" />
                <PrePosition X="0.3563" Y="0.2750" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="stage/ui/elixer.png" Plist="assets.plist" />
              </NodeObjectData>
              <NodeObjectData Name="Text_PlayersCoin" ActionTag="53" FrameEvent="" Tag="51" ObjectIndex="13" FontSize="16" LabelText="10 000" ctype="TextObjectData">
                <Position X="117.0000" Y="88.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="78.0000" Y="18.0000" />
                <PrePosition X="0.7312" Y="0.7333" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="Text_PlayersElixer" ActionTag="54" FrameEvent="" Tag="52" ObjectIndex="14" FontSize="16" LabelText="10 000" ctype="TextObjectData">
                <Position X="114.0000" Y="32.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="78.0000" Y="18.0000" />
                <PrePosition X="0.7125" Y="0.2667" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="Text_8_Copy_0_Copy" ActionTag="55" FrameEvent="" Tag="53" ObjectIndex="15" FontSize="10" LabelText="Max: 751 000" TouchScaleChangeAble="True" ctype="TextObjectData">
                <Position X="89.0000" Y="110.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="93.0000" Y="11.0000" />
                <PrePosition X="0.5562" Y="0.9167" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="Text_8_Copy_0_Copy_Copy" ActionTag="56" FrameEvent="" Tag="54" ObjectIndex="16" FontSize="10" LabelText="Max: 751 000" TouchScaleChangeAble="True" ctype="TextObjectData">
                <Position X="88.0000" Y="55.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="93.0000" Y="11.0000" />
                <PrePosition X="0.5500" Y="0.4583" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
            </Children>
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </NodeObjectData>
          <NodeObjectData Name="OverallDamagePanel" CanEdit="False" ActionTag="64" FrameEvent="" Tag="62" ObjectIndex="13" TouchEnable="True" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
            <Position X="764.3737" Y="173.1256" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <AnchorPoint />
            <CColor A="255" R="255" G="255" B="255" />
            <Size X="180.0000" Y="110.0000" />
            <PrePosition X="0.7962" Y="0.2705" />
            <PreSize X="0.1875" Y="0.1719" />
            <Children>
              <NodeObjectData Name="Text_DamagePercent_Shadow" ActionTag="1454007507" FrameEvent="" Tag="51" ObjectIndex="30" FontSize="22" LabelText="100%" HorizontalAlignmentType="HT_Right" ctype="TextObjectData">
                <Position X="96.5001" Y="19.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="25" G="25" B="25" />
                <Size X="75.0000" Y="24.0000" />
                <PrePosition X="0.5361" Y="0.1727" />
                <PreSize X="0.4667" Y="0.2182" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="Text_DamagePercent" ActionTag="67" FrameEvent="" Tag="65" ObjectIndex="21" FontSize="22" LabelText="100%" HorizontalAlignmentType="HT_Right" ctype="TextObjectData">
                <Position X="94.5001" Y="22.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="75.0000" Y="24.0000" />
                <PrePosition X="0.5250" Y="0.2000" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="Text_OverallDamage_Shadow" ActionTag="1038270382" FrameEvent="" Tag="50" ObjectIndex="29" FontSize="14" LabelText="Overall Damage" ctype="TextObjectData">
                <Position X="84.0000" Y="82.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="25" G="25" B="25" />
                <Size X="166.0000" Y="15.0000" />
                <PrePosition X="0.5250" Y="0.8200" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="stage_battle_result_silver_star_Copy" ActionTag="-130159365" FrameEvent="" Tag="299" ObjectIndex="34" ctype="SpriteObjectData">
                <Position X="51.7347" Y="56.5292" />
                <Scale ScaleX="0.2000" ScaleY="0.2000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="0" G="0" B="0" />
                <Size X="176.0000" Y="169.0000" />
                <PrePosition X="0.2874" Y="0.5139" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="stage/battle_result/silver_star.png" Plist="assets.plist" />
              </NodeObjectData>
              <NodeObjectData Name="stage_battle_result_silver_star" ActionTag="-140327131" FrameEvent="" Tag="298" ObjectIndex="33" ctype="SpriteObjectData">
                <Position X="125.3916" Y="56.5292" />
                <Scale ScaleX="0.2000" ScaleY="0.2000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="0" G="0" B="0" />
                <Size X="176.0000" Y="169.0000" />
                <PrePosition X="0.6966" Y="0.5139" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="stage/battle_result/silver_star.png" Plist="assets.plist" />
              </NodeObjectData>
              <NodeObjectData Name="stage_battle_result_silver_star_Copy_Copy" ActionTag="-1749027724" FrameEvent="" Tag="300" ObjectIndex="35" ctype="SpriteObjectData">
                <Position X="88.5632" Y="56.5292" />
                <Scale ScaleX="0.2000" ScaleY="0.2000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="0" G="0" B="0" />
                <Size X="176.0000" Y="169.0000" />
                <PrePosition X="0.4920" Y="0.5139" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="stage/battle_result/silver_star.png" Plist="assets.plist" />
              </NodeObjectData>
              <NodeObjectData Name="Text_OverallDamage" ActionTag="69" FrameEvent="" Tag="67" ObjectIndex="23" FontSize="14" LabelText="Overall Damage" ctype="TextObjectData">
                <Position X="88.3130" Y="85.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="166.0000" Y="15.0000" />
                <PrePosition X="0.4906" Y="0.7727" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
            </Children>
            <SingleColor A="255" R="37" G="37" B="37" />
            <FirstColor A="255" R="37" G="37" B="37" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </NodeObjectData>
          <NodeObjectData Name="Panel_Message" ActionTag="-753520007" FrameEvent="" Tag="75" ObjectIndex="14" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
            <Position X="173.0000" Y="159.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <AnchorPoint />
            <CColor A="255" R="255" G="255" B="255" />
            <Size X="580.0000" Y="120.0000" />
            <PrePosition X="0.1802" Y="0.2484" />
            <PreSize X="0.6042" Y="0.1875" />
            <Children>
              <NodeObjectData Name="Text_Message_Shadow" ActionTag="-1787832605" FrameEvent="" Tag="74" ObjectIndex="33" FontSize="24" LabelText="Tap or press and hold to deploy&#xA;troops&#xA;" HorizontalAlignmentType="HT_Center" ctype="TextObjectData">
                <Position X="290.0000" Y="76.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="16" G="16" B="16" />
                <Size X="569.0000" Y="52.0000" />
                <PrePosition X="0.5000" Y="0.6333" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="Text_Message" ActionTag="759226594" FrameEvent="" Tag="73" ObjectIndex="32" FontSize="24" LabelText="Tap or press and hold to deploy&#xA;troops&#xA;" HorizontalAlignmentType="HT_Center" ctype="TextObjectData">
                <Position X="287.0000" Y="78.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="569.0000" Y="52.0000" />
                <PrePosition X="0.4948" Y="0.6500" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
            </Children>
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </NodeObjectData>
          <NodeObjectData Name="Panel_Warn" ActionTag="-732399591" FrameEvent="" Tag="76" ObjectIndex="15" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
            <Position X="174.0000" Y="392.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <AnchorPoint />
            <CColor A="255" R="255" G="255" B="255" />
            <Size X="580.0000" Y="120.0000" />
            <PrePosition X="0.1813" Y="0.6125" />
            <PreSize X="0.0000" Y="0.0000" />
            <Children>
              <NodeObjectData Name="Text_Warn_Shadow" ActionTag="-1692190485" FrameEvent="" Tag="77" ObjectIndex="34" FontSize="20" LabelText="You cannot deploy troops on the red area!" HorizontalAlignmentType="HT_Center" ctype="TextObjectData">
                <Position X="290.0000" Y="80.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="16" G="16" B="16" />
                <Size X="634.0000" Y="22.0000" />
                <PrePosition X="0.5000" Y="0.6667" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="Text_Warn" ActionTag="2056265525" FrameEvent="" Tag="78" ObjectIndex="35" FontSize="20" LabelText="You cannot deploy troops on the red area!" HorizontalAlignmentType="HT_Center" ctype="TextObjectData">
                <Position X="287.0000" Y="82.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="185" G="0" B="0" />
                <Size X="634.0000" Y="22.0000" />
                <PrePosition X="0.4948" Y="0.6833" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
            </Children>
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </NodeObjectData>
        </Children>
        <SingleColor A="255" R="0" G="0" B="0" />
        <FirstColor A="255" R="0" G="0" B="0" />
        <EndColor A="255" R="255" G="255" B="255" />
        <ColorVector ScaleX="1.0000" ScaleY="0.0000" />
      </ObjectData>
    </Content>
  </Content>
</GameProjectFile>