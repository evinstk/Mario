<?xml version="1.0" encoding="UTF-8"?>
<tileset name="sprites" tilewidth="16" tileheight="16" spacing="2" margin="1" tilecount="4" columns="4">
 <image source="../images/sprites.png" width="72" height="18"/>
 <tile id="0">
  <properties>
   <property name="collider" value="mario"/>
  </properties>
  <objectgroup draworder="index">
   <object id="4" x="1" y="0" width="13" height="16"/>
  </objectgroup>
 </tile>
 <tile id="1">
  <properties>
   <property name="animctrl" value="mario"/>
   <property name="motion" value="walk-right"/>
  </properties>
  <animation>
   <frame tileid="1" duration="100"/>
   <frame tileid="2" duration="100"/>
   <frame tileid="3" duration="100"/>
   <frame tileid="2" duration="100"/>
  </animation>
 </tile>
</tileset>
