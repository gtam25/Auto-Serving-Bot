-- phpMyAdmin SQL Dump
-- version 4.6.4
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Oct 17, 2016 at 02:24 PM
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
(31, 4, 'Paneer do pyaaza-2,Dal Makhni-2,Veg. Biryani-2,Murg Dum Biryani-2,Butter Naan-2,Tandoori Roti-2', '1240.00', '2016-10-09 15:07:59'),
(30, 2, 'Murg Dum Biryani-2,Chicken Tikka Biryani-2,Tandoori Roti-2,Panner masala-2,Rice-4', '1346.00', '2016-10-09 14:58:32'),
(32, 7, 'Shahi Paneer Biryani-2,Chicken Tikka Biryani-4,Gulab Jamun-2,Rice-2', '1460.00', '2016-10-09 15:08:25');

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
  MODIFY `orderID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=33;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
