-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.7.11-log - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL Version:             9.3.0.4984
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- Dumping database structure for avalon
CREATE DATABASE IF NOT EXISTS `avalon` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `avalon`;


-- Dumping structure for table avalon.bots
CREATE TABLE IF NOT EXISTS `bots` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `MeshID` int(11) DEFAULT NULL,
  `X` int(11) DEFAULT NULL,
  `Y` int(11) DEFAULT NULL,
  `Z` int(11) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- Dumping data for table avalon.bots: ~3 rows (approximately)
/*!40000 ALTER TABLE `bots` DISABLE KEYS */;
INSERT INTO `bots` (`ID`, `MeshID`, `X`, `Y`, `Z`) VALUES
	(1, 35, -98, 26, -344),
	(2, 35, 1, 26, -354),
	(3, 35, 104, 26, -211);
/*!40000 ALTER TABLE `bots` ENABLE KEYS */;


-- Dumping structure for table avalon.bounds
CREATE TABLE IF NOT EXISTS `bounds` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `x` float NOT NULL DEFAULT '0',
  `y` float NOT NULL DEFAULT '0',
  `z` float NOT NULL DEFAULT '0',
  `width` float NOT NULL DEFAULT '0',
  `height` float NOT NULL DEFAULT '0',
  `deep` float NOT NULL DEFAULT '0',
  `RotX` float NOT NULL DEFAULT '0',
  `RotY` float NOT NULL DEFAULT '0',
  `RotZ` float NOT NULL DEFAULT '0',
  `type` int(11) NOT NULL DEFAULT '0',
  `worldID` int(11) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- Dumping data for table avalon.bounds: ~2 rows (approximately)
/*!40000 ALTER TABLE `bounds` DISABLE KEYS */;
INSERT INTO `bounds` (`ID`, `x`, `y`, `z`, `width`, `height`, `deep`, `RotX`, `RotY`, `RotZ`, `type`, `worldID`) VALUES
	(1, -96.6915, 23.4701, -212.406, 5.42641, 6.2628, 5.42639, 0, 0, 0, 0, 5),
	(2, -84.4742, 27.8388, -235.075, 25.0001, 15, 2.99991, 0, 45, 0, 0, 6),
	(3, -72.3497, 21.3433, -342.413, 2, 2, 2, 0, 0, 0, 1, 7);
/*!40000 ALTER TABLE `bounds` ENABLE KEYS */;


-- Dumping structure for table avalon.models
CREATE TABLE IF NOT EXISTS `models` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Name` varchar(100) NOT NULL DEFAULT '0',
  `MeshName` varchar(100) NOT NULL DEFAULT '0',
  `Width` float NOT NULL DEFAULT '0',
  `Height` float NOT NULL DEFAULT '0',
  `Depth` float NOT NULL DEFAULT '0',
  `Type` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `MeshName` (`MeshName`),
  UNIQUE KEY `Name` (`Name`)
) ENGINE=InnoDB AUTO_INCREMENT=36 DEFAULT CHARSET=utf8;

-- Dumping data for table avalon.models: ~35 rows (approximately)
/*!40000 ALTER TABLE `models` DISABLE KEYS */;
INSERT INTO `models` (`ID`, `Name`, `MeshName`, `Width`, `Height`, `Depth`, `Type`) VALUES
	(1, 'Barrel', 'Barrel.mesh', 5.4264, 6.2628, 5.4264, 0),
	(2, 'Column', 'column.mesh', 47.94, 450.84, 47.94, 0),
	(3, 'Fish', 'fish.mesh', 7.51718, 3.13848, 3.51827, 0),
	(4, 'Penguin', 'penguin.mesh', 63.0057, 47.9906, 49.2714, 0),
	(5, 'Robot', 'robot.mesh', 32.7677, 103.706, 50.0744, 0),
	(6, 'Cottage', 'tudorhouse.mesh', 689.504, 1143.27, 747.569, 0),
	(7, 'Pallet', 'WoodPallet.mesh', 7.34064, 0.819923, 7.20754, 0),
	(8, 'Barrier1', 'barrier1.mesh', 2.04, 1.16586, 2.37536, 0),
	(9, 'Container', 'container.mesh', 3.85968, 1.785, 2.04, 0),
	(10, 'nxogre.playpen', 'nxogre.playpen.mesh', 10.2, 2.55, 10.2, 0),
	(11, 'nxogre.platform', 'nxogre.platform.mesh', 108.12, 5.30075, 108.12, 0),
	(12, 'weight1', 'weight1.mesh', 3.06204, 0.612, 1.02, 0),
	(13, 'weight2', 'weight2.mesh', 6.1404, 2.05632, 2.05632, 0),
	(14, 'Mikki', 'Mikki.mesh', 68.8093, 314.871, 105.8, 0),
	(15, '3BarFence', '3BarFence.mesh', 1517.27, 603.169, 61.4419, 0),
	(16, '3LeggedStool', '3LeggedStool.mesh', 0, 0, 0, 0),
	(17, '3RungSteps', '3RungSteps.mesh', 0, 0, 0, 0),
	(18, 'BassBroom', 'BassBroom.mesh', 0, 0, 0, 0),
	(19, 'BeerBarrel', 'BeerBarrel.mesh', 0, 0, 0, 0),
	(20, 'Cupboard', 'Cupboard.mesh', 0, 0, 0, 0),
	(21, 'BritishPostBox', 'BritishPostBox.mesh', 0, 0, 0, 0),
	(22, 'EarthGlobe', 'EarthGlobe.mesh', 0, 0, 0, 0),
	(23, 'GardenFork', 'GardenFork.mesh', 0, 0, 0, 0),
	(24, 'GardenSpade', 'GardenSpade.mesh', 0, 0, 0, 0),
	(25, 'InsetPostBox', 'InsetPostBox.mesh', 0, 0, 0, 0),
	(26, 'LawnRake', 'LawnRake.mesh', 0, 0, 0, 0),
	(27, 'MetalBucket', 'MetalBucket.mesh', 0, 0, 0, 0),
	(28, 'MetalDustbin', 'MetalDustbin.mesh', 0, 0, 0, 0),
	(29, 'PoleLadder', 'PoleLadder.mesh', 0, 0, 0, 0),
	(30, 'RoundBackChair', 'RoundBackChair.mesh', 0, 0, 0, 0),
	(31, 'WheelieBin', 'WheelieBin.mesh', 0, 0, 0, 0),
	(32, 'WindowCleanersLadder', 'WindowCleanersLadder.mesh', 0, 0, 0, 0),
	(33, 'WoodenChair', 'WoodenChair.mesh', 0, 0, 0, 0),
	(34, 'cube.1m', 'cube.1m.mesh', 0, 0, 0, 0),
	(35, 'sinbad', 'sinbad.mesh', 0, 0, 0, 1);
/*!40000 ALTER TABLE `models` ENABLE KEYS */;


-- Dumping structure for table avalon.world
CREATE TABLE IF NOT EXISTS `world` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `ModelID` int(11) NOT NULL DEFAULT '0',
  `x` float NOT NULL DEFAULT '0',
  `y` float NOT NULL DEFAULT '0',
  `z` float NOT NULL DEFAULT '0',
  `Width` float NOT NULL DEFAULT '0',
  `Height` float NOT NULL DEFAULT '0',
  `Deep` float NOT NULL DEFAULT '0',
  `Scale` float NOT NULL DEFAULT '0',
  `RotX` float NOT NULL DEFAULT '0',
  `RotY` float NOT NULL DEFAULT '0',
  `RotZ` float NOT NULL DEFAULT '0',
  `Mass` float NOT NULL DEFAULT '0',
  `Type` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;

-- Dumping data for table avalon.world: ~4 rows (approximately)
/*!40000 ALTER TABLE `world` DISABLE KEYS */;
INSERT INTO `world` (`ID`, `ModelID`, `x`, `y`, `z`, `Width`, `Height`, `Deep`, `Scale`, `RotX`, `RotY`, `RotZ`, `Mass`, `Type`) VALUES
	(1, 6, -145.21, 68.3433, -378.199, 0.108774, 0.087468, 0.100325, 1, 0, 0, 0, 100, 0),
	(2, 6, -41.7296, 68.3433, -377.082, 0.108774, 0.087468, 0.100325, 1, 0, 0, 0, 100, 0),
	(4, 6, 67.6005, 68.3433, -344.699, 0.108774, 0.087468, 0.100325, 1, 0, -45, 0, 100, 0),
	(5, 1, -96.6915, 23.4701, -212.406, 1, 1, 1, 1, 0, 0, 0, 100, 0),
	(6, 8, -84.4742, 27.8388, -235.075, 12.2549, 12.866, 3.58669, 1, 0, 45, 0, 100, 0),
	(7, 27, -72.3497, 21.3433, -342.413, 0.003714, 0.003269, 0.003435, 1, 0, 0, 0, 100, 0);
/*!40000 ALTER TABLE `world` ENABLE KEYS */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
