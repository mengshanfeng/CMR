<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema">
	<!-- ********************************************************* -->
	<xsl:output method="html" indent="yes"/>

	<!-- ********************************************************* -->
	<xsl:template match="/">
		<html>
			<xsl:call-template name="CMRHtmlHeader"/>	
			<body>
				<xsl:apply-templates select="cmr/project"/>
				<xsl:apply-templates select="cmr/elements"/>
			</body>
		</html>
	</xsl:template>

	<!-- ********************************************************* -->
	<xsl:template match="elements">
		<xsl:apply-templates select="consts"/>
	</xsl:template>

	<!-- ********************************************************* -->
	<xsl:template match="consts">
		<div class='CMRConsts'>
			<h2>Constantes</h2>
			<xsl:apply-templates match="const"/>
		</div>
	</xsl:template>

	<!-- ********************************************************* -->
	<xsl:template match="const">
		<div class="CMRConst">
			<p class='CMRConstDoc'>
				<xsl:value-of select='@doc'/> <br/> (<i class='CMRConstDecl'><xsl:call-template name='CMRConstDecl'/></i>)
			</p>
			<p class='CMRConstDef'>\begin{align}
				<xsl:value-of select="@mathname"/> = <xsl:call-template name="CMRConstValue"/>
				\end{align}
			</p>
		</div>
	</xsl:template>

	<!-- ********************************************************* -->
	<xsl:template name="CMRConstDecl">
		<xsl:choose>
			<xsl:when test="@dims = 1"><xsl:value-of select="concat('const ',@type,' ',@longname,'[]')"/></xsl:when>
			<xsl:when test="@dims = 2"><xsl:value-of select="concat('const ',@type,' ',@longname,'[][]')"/></xsl:when>
			<xsl:otherwise><xsl:value-of select="concat('const ',@type,' ',@longname)"/></xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- ********************************************************* -->
	<xsl:template name="CMRConstValue">
		<xsl:choose>
			<xsl:when test="@dims > 0">\begin{pmatrix} <xsl:value-of select="."/> \end{pmatrix}</xsl:when>
			<xsl:otherwise><xsl:value-of select='.'/></xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	
	<!-- ********************************************************* -->
	<xsl:template match="project">
		<h1><xsl:value-of select='name'/></h1>
		<div class="CMRInfoSection">
			<h2>Description</h2>		
			<table>
				<tr>
					<td>Version :</td>
					<td><xsl:value-of select="version"/></td>
				</tr>
				<tr>
					<td>Description :</td>
					<td><xsl:value-of select="doc"/></td>
				</tr>
				<tr>
				<td>Auteurs :</td>
					<td>
							<xsl:for-each select='authors/author'>
								<div><xsl:value-of select='.'/> (<a><xsl:attribute name='href'>mailto:<xsl:value-of select='@mail'/></xsl:attribute><xsl:value-of select='@mail'/></a>)</div>
							</xsl:for-each>
					</td>
				</tr><tr>
					<td>Capture :</td>
					<td>
						<img class="CMRInfoScreen">
							<xsl:attribute name="src"><xsl:value-of select='screen'/></xsl:attribute>
						</img>
					</td>
				</tr>
			</table>
		</div>
	</xsl:template>

	<!-- ********************************************************* -->
	<xsl:template name="CMRHtmlHeader">
		<head>
			<meta http-equiv="content-type" content="text/html; charset=utf-8"/>
			<title>LBM : Lattice Boltzman Method</title>
			<script type="text/x-mathjax-config">
						MathJax.Hub.Config({
							extensions: ["tex2jax.js"],
							jax: ["input/TeX","output/HTML-CSS"],
							tex2jax: {inlineMath: [["$","$"],["\\(","\\)"]]}
						});
			</script>
			<script type="text/javascript" src="./extern-deps/mathjax/MathJax.js?config=TeX-AMS-MML_HTMLorMML"/>
			<script language="javascript" type="text/javascript" src="./extern-deps/jpqplot/jquery.min.js"/>
			<script language="javascript" type="text/javascript" src="./extern-deps/jpqplot/jquery.jqplot.min.js"/>
			<link rel="stylesheet" type="text/css" href="./extern-deps/jpqplot/jquery.jqplot.css"/>
			<xsl:call-template name="CMRCssStyle"/>
		</head>
	</xsl:template>

	<!-- ********************************************************* -->
	<xsl:template name="CMRCssStyle">
		<style type="text/css">
			body {
				background-color:#DADCDE;
			}
			h1 {
				text-align:center;
				font:"Lucida Grande","Lucida Sans Unicode","DejaVu Sans","Lucida,Arial","Helvetica","sans-serif";
				color:#333333;
				font-size: 1.4em;
				font-weight: bold;
			}
			.CMREquation {
				font-size:120%;
				text-align:left;
				font-family: "Courier", monospace;
			}
			.CMROperation, .CMRConsts, .CMRDefinition, .CMRInfoSection {
				background-color:white;
				border-radius: 15px;
				box-shadow: 0px 0px 5px #888888;
				padding:5px;
				max-width:1400px;
				margin:10px;
				margin-left:auto;
				margin-right:auto;
			}
			.CMRConsts , .CMRInfoSection {
				background-color:#F5F5F5;
			}
			.CMRDefinition {
				background-color:#D2E6FF;
			}
			.CMRFinalDefinition{
				border:1px solid gray;
			}
			h2 {
				text-align:center;
				font:"Lucida Grande","Lucida Sans Unicode","DejaVu Sans","Lucida,Arial","Helvetica","sans-serif";
				color:#333333;
				font-size: 1.4em;
				font-weight: bold;
			}
			table.CMRAccess {
				border-collapse:collapse;
				border:1px solid #333333;
			}
			table.CMRAccess tr td{
				border:1px dotted #333333;
				width:25px;
				height:25px;
				text-align:center;
			}
			table.CMRAccess .Write {
				background-color:#DE8283;
			}
			table.CMRAccess .Read {
				background-color:#C1FFB6;
			}
			table.CMRAccess .ReadWrite {
				background-color:#E7B3FF;
			}
			.CMRConstDecl {
				color: gray;
			}
		</style>
	</xsl:template>
</xsl:stylesheet>

