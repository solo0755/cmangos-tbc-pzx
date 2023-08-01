/*
Navicat MySQL Data Transfer

Source Server         : local
Source Server Version : 50726
Source Host           : 127.0.0.1:3306
Source Database       : tbccharacters

Target Server Type    : MYSQL
Target Server Version : 50726
File Encoding         : 65001

Date: 2023-08-02 00:06:35
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for _pzx_cardtree
-- ----------------------------
DROP TABLE IF EXISTS `_pzx_cardtree`;
CREATE TABLE `_pzx_cardtree` (
  `id` int(11) NOT NULL,
  `pid` int(11) NOT NULL DEFAULT '0',
  `name` varchar(255) NOT NULL,
  `unionID` int(11) DEFAULT '0',
  `needval` tinyint(4) DEFAULT '0',
  `raceMask` int(11) NOT NULL DEFAULT '0',
  `type` tinyint(4) DEFAULT '0',
  `param1` varchar(255) DEFAULT NULL,
  `param2` varchar(255) DEFAULT NULL,
  `param3` varchar(255) DEFAULT NULL,
  `param4` varchar(255) DEFAULT NULL,
  `param5` varchar(255) DEFAULT NULL,
  `unioncheck` int(255) DEFAULT '0',
  `popMenu` int(255) DEFAULT '0',
  `iconID` int(11) DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of _pzx_cardtree
-- ----------------------------
INSERT INTO `_pzx_cardtree` VALUES ('101', '0', '物品', '0', '0', '0', '1', '19019', '1', null, null, null, '0', '0', '0');
INSERT INTO `_pzx_cardtree` VALUES ('102', '0', '|cFF990066|TInterface\\\\ICONS\\\\INV_Holiday_Christmas_Present_02.blp:30|t|r 传送点~~(|cffff0000必选|r)', '0', '0', '0', '2', '0', '0', null, null, null, '0', '0', '0');
INSERT INTO `_pzx_cardtree` VALUES ('103', '0', '重载配置', '0', '0', '0', '2', '19019', '1', null, '', null, '0', '0', '0');
INSERT INTO `_pzx_cardtree` VALUES ('10201', '102', '主城传送', '0', '0', '0', '0', '0', '0', null, null, null, '0', '0', '0');
INSERT INTO `_pzx_cardtree` VALUES ('10202', '102', '副本传送', '0', '0', '0', '0', '0', '0', null, '', null, '0', '0', '0');
INSERT INTO `_pzx_cardtree` VALUES ('1020101', '10201', '奥格瑞玛', '0', '0', '0', '11', '1437.0', '-4421.0 ', '25.24 ', '1', null, '0', '0', '0');
INSERT INTO `_pzx_cardtree` VALUES ('1020201', '10202', '奥妮克希亚的巢穴', '0', '0', '0', '11', '-4682.39 ', '-3709.85 ', '46.79', '1', null, '0', '0', '0');
