<GameProjectFile>
  <PropertyGroup Type="Node" Name="UnitArcherAttackNorth" ID="82a33b50-5ab8-4294-8cf9-276c0963c976" Version="2.0.0.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="15" Speed="1">
        <Timeline ActionTag="99" FrameType="TextureFrame">
          <TextureFrame FrameIndex="0">
            <TextureFile Type="PlistSubImage" Path="unit/archer/attack/north/01.png" Plist="asset.plist" />
          </TextureFrame>
          <TextureFrame FrameIndex="5">
            <TextureFile Type="PlistSubImage" Path="unit/archer/attack/north/02.png" Plist="asset.plist" />
          </TextureFrame>
          <TextureFrame FrameIndex="10">
            <TextureFile Type="PlistSubImage" Path="unit/archer/attack/north/03.png" Plist="asset.plist" />
          </TextureFrame>
          <TextureFrame FrameIndex="15">
            <TextureFile Type="PlistSubImage" Path="unit/archer/attack/north/03.png" Plist="asset.plist" />
          </TextureFrame>
        </Timeline>
      </Animation>
      <ObjectData Name="Node" CanEdit="False" FrameEvent="" ctype="SingleNodeObjectData">
        <Position X="0" Y="0" />
        <Scale ScaleX="1" ScaleY="1" />
        <AnchorPoint />
        <CColor A="255" R="255" G="255" B="255" />
        <Size X="0" Y="0" />
        <PrePosition X="0" Y="0" />
        <PreSize X="0" Y="0" />
        <Children>
          <NodeObjectData Name="Sprite_1" ActionTag="99" FrameEvent="" Tag="50" ObjectIndex="1" ctype="SpriteObjectData">
            <Position X="0" Y="0" />
            <Scale ScaleX="1" ScaleY="1" />
            <AnchorPoint ScaleX="0.5" ScaleY="0.5" />
            <CColor A="255" R="255" G="255" B="255" />
            <Size X="23" Y="36" />
            <PrePosition X="0" Y="0" />
            <PreSize X="0" Y="0" />
            <FileData Type="PlistSubImage" Path="unit/archer/attack/north/03.png" Plist="asset.plist" />
          </NodeObjectData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameProjectFile>