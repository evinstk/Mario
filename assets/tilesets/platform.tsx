<?xml version="1.0" encoding="UTF-8"?>
<tileset name="mario-tileset" tilewidth="16" tileheight="16" tilecount="924" columns="33">
 <image source="../images/mario-tileset.png" width="528" height="448"/>
 <tile id="0">
  <properties>
   <property name="solid" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="1">
  <properties>
   <property name="collider" value="brick"/>
  </properties>
  <objectgroup draworder="index">
   <object id="2" x="0" y="0" width="16" height="16"/>
  </objectgroup>
 </tile>
 <tile id="3">
  <properties>
   <property name="name" value="empty-block"/>
  </properties>
 </tile>
 <tile id="24">
  <properties>
   <property name="animation" value="prize"/>
   <property name="collider" value="prize"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="0" y="0" width="16" height="16"/>
  </objectgroup>
  <animation>
   <frame tileid="24" duration="600"/>
   <frame tileid="25" duration="200"/>
   <frame tileid="26" duration="200"/>
   <frame tileid="25" duration="200"/>
  </animation>
 </tile>
 <tile id="57">
  <properties>
   <property name="animation" value="coin"/>
  </properties>
  <animation>
   <frame tileid="57" duration="100"/>
   <frame tileid="123" duration="100"/>
  </animation>
 </tile>
 <tile id="264">
  <properties>
   <property name="solid" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="265">
  <properties>
   <property name="solid" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="297">
  <properties>
   <property name="solid" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="298">
  <properties>
   <property name="solid" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="330">
  <properties>
   <property name="solid" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="331">
  <properties>
   <property name="solid" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="363">
  <properties>
   <property name="solid" type="bool" value="true"/>
  </properties>
 </tile>
 <tile id="364">
  <properties>
   <property name="solid" type="bool" value="true"/>
  </properties>
 </tile>
</tileset>
