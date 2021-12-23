<div id="top"></div>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->

<!--
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]
-->


<!-- PROJECT LOGO -->
<br />
<div align="center">
  <h3 align="center">Office Hour Simulation: Concurrency</h3>

  <p align="center"> CLI based simulator using C that simulates office hours held by a teacher for students from multiple classes by implementing <b>concurrency</b>. </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project
This is a hands-on experience with parallel programming and the difficulty of building correct parallel programs.

#### Simulation Scenario
You are tasked with helping your professor schedule his office hours. The professor is teaching 2 classes this semester, class A and class B, and is holding shared office hours for both classes in his office. The professor can  have a large number of students showing up for his office hours, so he decides to impose several restrictions. 

* The professors’s office has only 3 seats, so no more than 3 students are allowed to simultaneously enter the professor’s office. When the office is full and new students arrive they have to wait outside the office.
* The professor gets confused when helping students from class A and class B at the same time. He decides that while students from class A are in his office, no students from class B are allowed to enter, and the other way around. 
* The professor gets tired after answering too many questions. He decides that after helping 10 students he needs to take a break before he can help more students. So after the 10th student (counting since the last break) enters the professors office no more students are admitted into the office, until after the professors’s next break. Students that arrive while the professor is taking his break have to wait outside the office.
* In order to be fair to both classes after 5 consecutive students from a single class the professor will answer questions from a student from the other class.
*  if there is no student in the professor’s office and the professor is not currently taking a break an arriving student should not be forced to wait. Similarly, if an arriving student is compatible with the students currently in the office he should not be forced to wait, unless the professor is due for a break. 

This program:
* Handles more than 50 concurrent threads using the Pthreads API in Linux OS
* Implements functionalities of mutex and semaphores to avoid Deadlocks and Race condition

<p align="right">(<a href="#top">back to top</a>)</p>

### Built With

* [C]
* [Pthreads API]

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started

This simulation has been tested in a Linux OS, I recommend using Linux(RedHat or Ubuntu) while using this program. 

### Prerequisites

To run C++ or C, first install gcc compilers 
* buid-essential
  ```sh
  sudo apt install build-essential
  ```
* install manpage 
  ```sh
  sudo apt-get install manpages-dev
  ```

### Installation

The code compiles into four shared libraries and four test programs.  To build the code, change to your top level assignment directory and type: 

1. Requires to be compiled with pthread library to run
   ```sh
   gcc officehours.c -o office hours -lpthread 
   ```

   ```
<p align="right">(<a href="#top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage Examples

To run the program
 ```sh
    ./office hours testfile.txt 
   ```
    
#### Test File Example:

0 0 1<br>
0 0 1<br>
0 0 1<br>
0 2 1<br>
0 2 1<br>
0 2 1<br>
0 4 1<br>
0 4 1<br>
0 4 1<br>
0 6 1<br>
0 6 1<br>
0 6 1<br>

<p align="right">(<a href="#top">back to top</a>)</p>


<!-- ROADMAP -->
## Roadmap

- [x] The professors’s office has only 3 seats, so no more than 3 students are allowed to simultaneously enter the professor’s office. When the office is full and new students arrive they have to wait outside the office.
- [x] The professor gets confused when helping students from class A and class B at the same time. He decides that while students from class A are in his office, no students from class B are allowed to enter, and the other way around. 
- [x] The professor gets tired after answering too many questions. He decides that after helping 10 students he needs to take a break before he can help more students. So after the 10th student (counting since the last break) enters the professors office no more students are admitted into the office, until after the professors’s next break. Students that arrive while the professor is taking his break have to wait outside the office.
- [X] In order to be fair to both classes after 5 consecutive students from a single class the professor will answer questions from a student from the other class.
- [X] if there is no student in the professor’s office and the professor is not currently taking a break an arriving student should not be forced to wait. Similarly, if an arriving student is compatible with the students currently in the office he should not be forced to wait, unless the professor is due for a break. 

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Biraj Singh GC- [@birajsinghgc@gmail.com](birajsinghgc@gmail.com)

Project Link: [https://github.com/BirajSinghGCUTA/C-Heap](https://github.com/BirajSinghGCUTA/C-Heap)

<p align="right">(<a href="#top">back to top</a>)</p>
