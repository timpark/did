<html><head><title>did</title></head><body>
<?
$tasks = ['exe', 'med', 'study', 'clean'];
$arg = preg_replace("/[^a-z A-Z0-9]+/", "", $_POST['did']);
if ($arg != "") system("./did $arg");
echo "..." . $arg;
?>
<pre><? system("./did"); ?></pre><br/>
<form method="post" action="did.php">
&nbsp;&nbsp;Today:
<?foreach ($tasks as $task) echo "<button type=\"submit\" name=\"did\" value=\"" . $task . "\">&nbsp;&nbsp;&nbsp;" . $task . "&nbsp;&nbsp;&nbsp;</button>";?>
<br/><br/><br/>
&nbsp;&nbsp;&nbsp;Yest..:
<?foreach ($tasks as $task) echo "<button type=\"submit\" name=\"did\" value=\"" . $task . " yest\">&nbsp;&nbsp;&nbsp;" . $task . "&nbsp;&nbsp;&nbsp;</button>";?>
<br/><br/><br/>
<button type="submit" name="did" value="">&nbsp;&nbsp;&nbsp;refresh&nbsp;&nbsp;&nbsp;</button>
</form></body></html>
