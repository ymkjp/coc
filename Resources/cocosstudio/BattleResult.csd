<GameProjectFile>
  <PropertyGroup Type="Layer" Name="BattleResult" ID="b502f420-e5f8-444b-a8c0-4c56184477af" Version="2.0.2.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="200" Speed="1.0000">
        <Timeline ActionTag="-1582794760" FrameType="PositionFrame">
          <PointFrame FrameIndex="0" X="207.0000" Y="305.0000" />
        </Timeline>
        <Timeline ActionTag="-1582794760" FrameType="ScaleFrame">
          <PointFrame FrameIndex="0" X="10.0000" Y="10.0000" />
        </Timeline>
        <Timeline ActionTag="-1582794760" FrameType="RotationSkewFrame">
          <PointFrame FrameIndex="0" X="0.0000" Y="0.0000" />
          <PointFrame FrameIndex="200" X="359.9000" Y="359.9000" />
        </Timeline>
      </Animation>
      <ObjectData Name="Layer" CanEdit="False" FrameEvent="" ComboBoxIndex="1" ColorAngle="0.0000" ctype="PanelObjectData">
        <Position X="0.0000" Y="0.0000" />
        <Scale ScaleX="1.0000" ScaleY="1.0000" />
        <AnchorPoint />
        <CColor A="255" R="255" G="255" B="255" />
        <Size X="960.0000" Y="640.0000" />
        <PrePosition X="0.0000" Y="0.0000" />
        <PreSize X="0.0000" Y="0.0000" />
        <Children>
          <NodeObjectData Name="Panel_Background" ActionTag="169863224" FrameEvent="" Tag="76" ObjectIndex="4" TouchEnable="True" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
            <Position X="0.0000" Y="0.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <AnchorPoint />
            <CColor A="255" R="255" G="255" B="255" />
            <Size X="960.0000" Y="640.0000" />
            <PrePosition X="0.0000" Y="0.0000" />
            <PreSize X="1.0000" Y="1.0000" />
            <Children>
              <NodeObjectData Name="stage_battle_result_barbarianking_brack" CanEdit="False" ActionTag="888245552" FrameEvent="" Tag="71" ObjectIndex="2" ctype="SpriteObjectData">
                <Position X="765.0000" Y="340.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="0" G="0" B="0" />
                <Size X="314.0000" Y="358.0000" />
                <PrePosition X="0.7969" Y="0.5320" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="stage/battle_result/barbarianking_brack.png" Plist="assets.plist" />
              </NodeObjectData>
              <NodeObjectData Name="stage_battle_result_wizerd_black" CanEdit="False" ActionTag="756191313" FrameEvent="" Tag="72" ObjectIndex="3" ctype="SpriteObjectData">
                <Position X="148.0000" Y="340.5000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="0" G="0" B="0" />
                <Size X="287.0000" Y="386.0000" />
                <PrePosition X="0.1542" Y="0.5320" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="stage/battle_result/wizerd_black.png" Plist="assets.plist" />
              </NodeObjectData>
              <NodeObjectData Name="Bottom" ActionTag="1129870946" FrameEvent="" Tag="90" ObjectIndex="7" TouchEnable="True" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
                <Position X="0.0000" Y="-1.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="0" G="0" B="0" />
                <Size X="960.0000" Y="180.0000" />
                <PrePosition X="0.0000" Y="-0.0016" />
                <PreSize X="1.0000" Y="0.2813" />
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </NodeObjectData>
            </Children>
            <SingleColor A="255" R="30" G="30" B="30" />
            <FirstColor A="255" R="30" G="30" B="30" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </NodeObjectData>
          <NodeObjectData Name="Panel_ReturnHome" ActionTag="1700139942" FrameEvent="" Tag="63" ObjectIndex="1" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
            <Position X="469.0000" Y="109.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <AnchorPoint ScaleX="0.5000" />
            <CColor A="255" R="255" G="255" B="255" />
            <Size X="138.0000" Y="76.0000" />
            <PrePosition X="0.4885" Y="0.1703" />
            <PreSize X="0.1437" Y="0.1187" />
            <Children>
              <NodeObjectData Name="Button_ReturnHome" ActionTag="-1963118851" FrameEvent="" Tag="67" ObjectIndex="1" TouchEnable="True" FontSize="14" ButtonText="" Scale9Width="158" Scale9Height="75" ctype="ButtonObjectData">
                <Position X="-3.0000" Y="-0.0001" />
                <Scale ScaleX="1.0800" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="158.0000" Y="75.0000" />
                <PrePosition X="-0.0217" Y="0.0000" />
                <PreSize X="1.0290" Y="1.0000" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" />
                <PressedFileData Type="PlistSubImage" Path="stage/battle_result/GreenButtonPressed.png" Plist="assets.plist" />
                <NormalFileData Type="PlistSubImage" Path="stage/battle_result/GreenButton.png" Plist="assets.plist" />
              </NodeObjectData>
              <NodeObjectData Name="Text_ReturnHome_Shadow" ActionTag="42569196" FrameEvent="" Tag="66" ObjectIndex="2" FontSize="20" LabelText="Return&#xA;home" HorizontalAlignmentType="HT_Center" ctype="TextObjectData">
                <Position X="8.0000" Y="14.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="22" G="22" B="22" />
                <Size X="106.0000" Y="44.0000" />
                <PrePosition X="0.0580" Y="0.1842" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
              <NodeObjectData Name="Text_ReturnHome" ActionTag="-258055486" FrameEvent="" Tag="65" ObjectIndex="1" FontSize="20" LabelText="Return&#xA;home" HorizontalAlignmentType="HT_Center" ctype="TextObjectData">
                <Position X="6.0000" Y="16.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="106.0000" Y="44.0000" />
                <PrePosition X="0.0435" Y="0.2105" />
                <PreSize X="0.0000" Y="0.0000" />
                <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
              </NodeObjectData>
            </Children>
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </NodeObjectData>
          <NodeObjectData Name="Panel_MainResult" ActionTag="-1181834641" FrameEvent="" Tag="75" ObjectIndex="3" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
            <Position X="269.0000" Y="158.0000" />
            <Scale ScaleX="1.0000" ScaleY="1.0000" />
            <AnchorPoint />
            <CColor A="255" R="255" G="255" B="255" />
            <Size X="400.0000" Y="400.0000" />
            <PrePosition X="0.2802" Y="0.2469" />
            <PreSize X="0.4167" Y="0.6250" />
            <Children>
              <NodeObjectData Name="stage_battle_result_954.0" CanEdit="False" ActionTag="-1582794760" FrameEvent="" Tag="88" ObjectIndex="8" ctype="SpriteObjectData">
                <Position X="207.0000" Y="305.0000" />
                <Scale ScaleX="10.0000" ScaleY="10.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="41.0000" Y="59.0000" />
                <PrePosition X="0.5175" Y="0.7625" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="PlistSubImage" Path="stage/battle_result/background_bright.png" Plist="assets.plist" />
              </NodeObjectData>
              <NodeObjectData Name="Panel_Ribbon" CanEdit="False" ActionTag="-1764269216" FrameEvent="" Tag="77" ObjectIndex="5" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                <Position X="26.9996" Y="271.9995" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="320.0000" Y="80.0000" />
                <PrePosition X="0.0675" Y="0.6800" />
                <PreSize X="0.4000" Y="0.2000" />
                <Children>
                  <NodeObjectData Name="stage_battle_result_952.0_Copy" CanEdit="False" ActionTag="854844724" FrameEvent="" Tag="74" ObjectIndex="5" FlipX="True" ctype="SpriteObjectData">
                    <Position X="271.0000" Y="38.9989" />
                    <Scale ScaleX="1.2000" ScaleY="1.2000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="159.0000" Y="68.0000" />
                    <PrePosition X="0.8469" Y="0.4875" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/battle_result/ribbon.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="stage_battle_result_952.0" CanEdit="False" ActionTag="861673501" FrameEvent="" Tag="73" ObjectIndex="4" ctype="SpriteObjectData">
                    <Position X="81.0000" Y="39.0000" />
                    <Scale ScaleX="1.2000" ScaleY="1.2000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="159.0000" Y="68.0000" />
                    <PrePosition X="0.2531" Y="0.4875" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/battle_result/ribbon.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="Text_Victory_Shadow" CanEdit="False" ActionTag="-1939945564" FrameEvent="" Tag="87" ObjectIndex="11" FontSize="20" LabelText="Victory" ctype="TextObjectData">
                    <Position X="178.5000" Y="38.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="21" G="21" B="21" />
                    <Size X="110.0000" Y="22.0000" />
                    <PrePosition X="0.5578" Y="0.4812" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
                  </NodeObjectData>
                  <NodeObjectData Name="Text_Victory" ActionTag="-1485681391" FrameEvent="" Tag="86" ObjectIndex="10" FontSize="20" LabelText="Victory" ctype="TextObjectData">
                    <Position X="181.5000" Y="37.5000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="247" B="168" />
                    <Size X="110.0000" Y="22.0000" />
                    <PrePosition X="0.5672" Y="0.4688" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
                  </NodeObjectData>
                </Children>
                <SingleColor A="255" R="150" G="200" B="255" />
                <FirstColor A="255" R="150" G="200" B="255" />
                <EndColor A="255" R="255" G="255" B="255" />
                <ColorVector ScaleY="1.0000" />
              </NodeObjectData>
              <NodeObjectData Name="Panel_TotalDamage" CanEdit="False" ActionTag="-1213226167" FrameEvent="" Tag="83" ObjectIndex="6" TouchEnable="True" BackColorAlpha="102" ColorAngle="90.0000" ctype="PanelObjectData">
                <Position X="-196.0000" Y="37.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <AnchorPoint />
                <CColor A="255" R="255" G="255" B="255" />
                <Size X="160.0000" Y="60.0000" />
                <PrePosition X="-0.4900" Y="0.0925" />
                <PreSize X="0.1667" Y="0.0938" />
                <Children>
                  <NodeObjectData Name="stage_battle_result_1512.1" ActionTag="-64154672" Alpha="205" FrameEvent="" Tag="79" ObjectIndex="6" ctype="SpriteObjectData">
                    <Position X="398.0000" Y="324.9963" />
                    <Scale ScaleX="1.4000" ScaleY="1.4000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="0" G="0" B="0" />
                    <Size X="135.0000" Y="48.0000" />
                    <PrePosition X="2.4875" Y="5.4166" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/battle_result/stars.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="stage_battle_result_1512.1_Copy" ActionTag="-1481626295" FrameEvent="" Tag="80" ObjectIndex="7" ctype="SpriteObjectData">
                    <Position X="397.9996" Y="325.9963" />
                    <Scale ScaleX="1.4000" ScaleY="1.4000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="135.0000" Y="48.0000" />
                    <PrePosition X="0.4975" Y="0.8150" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="PlistSubImage" Path="stage/battle_result/stars.png" Plist="assets.plist" />
                  </NodeObjectData>
                  <NodeObjectData Name="Text_TotalDamage_Shadow" ActionTag="-265980536" FrameEvent="" Tag="82" ObjectIndex="7" FontSize="12" LabelText="Total damage:" ctype="TextObjectData">
                    <Position X="402.0000" Y="345.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="20" G="20" B="20" />
                    <Size X="124.0000" Y="13.0000" />
                    <PrePosition X="2.5125" Y="5.7500" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
                  </NodeObjectData>
                  <NodeObjectData Name="Text_TotalDamage" ActionTag="-368436074" FrameEvent="" Tag="81" ObjectIndex="6" FontSize="12" LabelText="Total damage:" ctype="TextObjectData">
                    <Position X="401.0000" Y="347.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <Size X="124.0000" Y="13.0000" />
                    <PrePosition X="2.5062" Y="5.7833" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
                  </NodeObjectData>
                  <NodeObjectData Name="Text_Percentage_Shadow" ActionTag="-640856056" FrameEvent="" Tag="85" ObjectIndex="9" FontSize="22" LabelText="100%" HorizontalAlignmentType="HT_Center" ctype="TextObjectData">
                    <Position X="406.0000" Y="315.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="18" G="18" B="18" />
                    <Size X="75.0000" Y="24.0000" />
                    <PrePosition X="2.5375" Y="5.2500" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FontResource Type="Normal" Path="fonts/Supercell-Magic_5.ttf" />
                  </NodeObjectData>
                  <NodeObjectData Name="Text_Percentage" ActionTag="-1771514206" FrameEvent="" Tag="84" ObjectIndex="8" FontSize="22" LabelText="100%" HorizontalAlignmentType="HT_Center" ctype="TextObjectData">
                    <Position X="403.0000" Y="317.0000" />
                    <Scale ScaleX="1.0000" ScaleY="1.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="163" G="255" B="184" />
                    <Size X="75.0000" Y="24.0000" />
                    <PrePosition X="2.5187" Y="5.2833" />
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