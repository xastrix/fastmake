<img align="left" src="https://github.com/xastrix/fastmake/blob/master/media/logo.png" width="120">
<h3>fastmake</h3>
<p>A fast build system for Visual Studio C/C++ projects</p>
<h2>Usage</h2>
<pre>fastmake &lt;path&gt;</pre>
<p align="right">Check <a href="https://github.com/xastrix/fastmake/tree/master/docs">docs/README.md</a> for instructions on how to use it</p>
<h2>Installing</h2>
<p>Clone the repository</p>
<pre>git clone --recursive https://github.com/xastrix/fastmake.git</pre>
<p>Generate VS project by premake5</p>
<pre>cd fastmake && premake5 vs20xx</pre>
<p>
  Launch <kbd>fastmake.sln</kbd> (Visual Studio Solution File) and compile project by pressing <kbd><kbd>CTRL</kbd>+<kbd>Shift</kbd>+<kbd>B</kbd></kbd>
</p>
<h3>Requirements</h3>
<ul>
  <li>Visual Studio</li>
  <li>Windows SDK</li>
  <li><a href="https://github.com/premake/premake-core">premake</a></li>
</ul>
<h2>Dependencies</h2>
<a href="https://github.com/open-source-parsers/jsoncpp">jsoncpp</a>