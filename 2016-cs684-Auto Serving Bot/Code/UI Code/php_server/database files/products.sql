-- phpMyAdmin SQL Dump
-- version 4.6.4
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Oct 17, 2016 at 02:28 PM
-- Server version: 5.7.14
-- PHP Version: 5.6.25

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `android_app`
--

-- --------------------------------------------------------

--
-- Table structure for table `products`
--

CREATE TABLE `products` (
  `pid` int(11) NOT NULL,
  `item` varchar(50) NOT NULL,
  `cost` decimal(10,2) NOT NULL,
  `description` text,
  `created_at` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `updated_at` timestamp NULL DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `products`
--

INSERT INTO `products` (`pid`, `item`, `cost`, `description`, `created_at`, `updated_at`) VALUES
(4, 'Shahi Paneer Biryani', '150.00', 'Loads of tender marinated paneer cubes', '2016-09-30 10:42:31', NULL),
(17, 'Paneer do pyaaza', '180.00', 'paneer cooked in gravy filled with onions and tomatos', '2016-10-01 10:29:43', NULL),
(12, 'Dal Makhni', '80.00', 'The best dal makhni in town', '2016-09-30 10:42:00', NULL),
(1, 'Veg. Biryani', '120.00', 'Assorted vegetables cooked with Indian spices, layered with yellow and white rice', '2016-09-30 10:40:07', NULL),
(13, 'Murg Dum Biryani', '200.00', 'The quinessential classic boneless chicken biryani', '2016-09-30 10:43:16', NULL),
(14, 'Chicken Tikka Biryani', '250.00', 'Tikka style chicken in a biryani. A whole new experience', '2016-09-30 10:43:43', NULL),
(15, 'Butter Naan', '25.00', 'Indian Naan', '2016-09-30 10:44:17', NULL),
(16, 'Tandoori Roti', '15.00', 'Indian roti cooked in tandoor', '2016-09-30 10:44:43', NULL),
(18, 'Panner masala', '88.00', 'Panner cooked in rich gravy', '2016-10-01 12:31:47', NULL),
(19, 'Gulab Jamun', '20.00', 'Indian sweet', '2016-10-01 13:20:46', NULL),
(20, 'Rice', '60.00', 'Freshly cooked basmati rice ', '2016-10-04 10:45:07', NULL),
(21, 'Boondi Raita', '80.00', 'Crisp boondi mixed in a perfectly spiced thick curd', '2016-10-09 15:09:52', NULL),
(22, 'Curd', '50.00', 'Fresh plain curd', '2016-10-09 15:10:27', NULL);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `products`
--
ALTER TABLE `products`
  ADD PRIMARY KEY (`pid`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `products`
--
ALTER TABLE `products`
  MODIFY `pid` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=23;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
