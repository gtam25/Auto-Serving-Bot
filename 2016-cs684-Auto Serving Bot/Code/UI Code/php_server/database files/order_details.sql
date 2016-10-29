-- phpMyAdmin SQL Dump
-- version 4.6.4
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Oct 27, 2016 at 05:15 AM
-- Server version: 5.7.14
-- PHP Version: 5.6.25

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `order_details`
--

-- --------------------------------------------------------

--
-- Table structure for table `orderstatus`
--

CREATE TABLE `orderstatus` (
  `orderID` int(11) NOT NULL,
  `tablenumber` int(11) NOT NULL,
  `orderdescr` text,
  `totalcost` decimal(10,2) DEFAULT NULL,
  `created_at` timestamp NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `orderstatus`
--

INSERT INTO `orderstatus` (`orderID`, `tablenumber`, `orderdescr`, `totalcost`, `created_at`) VALUES
(85, 7, 'Murg Dum Biryani-5', '1000.00', '2016-10-23 15:06:28'),
(117, 6, 'Veg. Biryani-1,Shahi Paneer Biryani-2 ', '420.00', '2016-10-24 09:02:42'),
(87, 2, 'Veg. Biryani-1', '120.00', '2016-10-23 15:24:21'),
(88, 3, 'Veg. Biryani-1', '120.00', '2016-10-23 15:24:24'),
(89, 4, 'Veg. Biryani-1', '120.00', '2016-10-23 15:24:27'),
(90, 5, 'Veg. Biryani-1', '120.00', '2016-10-23 15:24:30'),
(91, 6, 'Veg. Biryani-1', '120.00', '2016-10-23 15:24:32'),
(121, 9, 'Veg. Biryani-4,Murg Dum Biryani-1', '680.00', '2016-10-24 11:07:52'),
(114, 9, 'Veg. Biryani-4', '480.00', '2016-10-24 08:08:30'),
(115, 9, 'Veg. Biryani-4', '480.00', '2016-10-24 08:08:40');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `orderstatus`
--
ALTER TABLE `orderstatus`
  ADD PRIMARY KEY (`orderID`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `orderstatus`
--
ALTER TABLE `orderstatus`
  MODIFY `orderID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=123;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
