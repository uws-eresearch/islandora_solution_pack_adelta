<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
xmlns:mods="http://www.loc.gov/mods/v3" exclude-result-prefixes="mods">

<xsl:variable name="lowerCase" select="'abcdefghijklmnopqrstuvwxyz'"/>
<xsl:variable name="upperCase" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'"/>
 
	<xsl:template match="/mods:mods">
		<!-- <h2>Description</h2>
		<p>Hello</p>
		<xsl:variable name="title" select="mods:titleInfo/mods:title"></xsl:variable>
		<p class="{$title}"><xsl:value-of select="concat(translate(substring($title,1,1), $lowerCase, $upperCase),
									substring($title,2))"/></p>
		<p class="{$title}"><xsl:value-of select="$title"></xsl:value-of></p> -->
		<dt class="title first">Title</dt>
		<dd class="title first"><xsl:value-of select="mods:titleInfo/mods:title"></xsl:value-of></dd>
		<dt class="creator">Creator</dt>
		<dd class="creator">
		<span property="dc:creator">
		<xsl:attribute name="href"><xsl:value-of select="mods:name[mods:role/mods:roleTerm='Author']/@uri"/></xsl:attribute></span>
		<span vocab="http://xmlns.com/foaf/0.1/" typeof="Person">
		<xsl:attribute name="about"><xsl:value-of select="mods:name[mods:role/mods:roleTerm='Author']/@uri"/></xsl:attribute>
    	<span property="name"><xsl:value-of select="mods:name/mods:namePart[following-sibling::mods:role/mods:roleTerm='Author']"></xsl:value-of></span></span>
		</dd>
		<dt class="unique_id">Unique id</dt>
		<dd class="unique_id">{{unique_id}}</dd>
		<dt class="description">Description</dt>
		<dd class="description"><xsl:value-of select="mods:abstract[@type='description']"></xsl:value-of></dd>
		<dt class="artist_stmt">Artist Statement</dt>
		<dd class="artist_stmt"><xsl:value-of select="mods:abstract[@type='artist_stmt']"></xsl:value-of></dd>
		<dt class="language">Language</dt>
		<dd class="language"><xsl:value-of select="mods:language"></xsl:value-of></dd>
		<dt class="date">Date</dt>
		<dd class="date"><xsl:value-of select="mods:originInfo/mods:dateIssued"></xsl:value-of></dd>
		<!-- Show publisher field only if it has a value -->
		<xsl:if test="not(mods:originInfo/mods:publisher='')">
			<dt class="publisher">Publisher</dt>
			<dd class="publisher"><xsl:value-of select="mods:originInfo/mods:publisher"></xsl:value-of></dd>
		</xsl:if>
		<dt class="internetMediaType">Medium</dt>
		<dd class="internetMediaType"><xsl:value-of select="mods:physicalDescription/mods:internetMediaType"></xsl:value-of></dd>
		<dt class="entry_author">Entry author</dt>
		<dd class="entry_author"><xsl:value-of select="mods:name/mods:namePart[preceding-sibling::mods:role/mods:roleTerm='entry_author']"></xsl:value-of></dd>
		<dt class="url">URL</dt>
		<dd class="url"><xsl:value-of select="mods:location/mods:url"></xsl:value-of></dd>
		<!-- Show isbn field only if it has a value -->
		<xsl:if test="not(mods:identifier[@type='isbn']='')">
			<dt class="isbn">ISBN</dt>
			<dd class="isbn"><xsl:value-of select="mods:identifier[@type='isbn']"></xsl:value-of></dd>
		</xsl:if>
		<xsl:if test="not(mods:name/mods:namePart[preceding-sibling::mods:role/mods:roleTerm='translator']='')">
			<dt class="translator">Translator</dt>
			<dd class="translator"><xsl:value-of select="mods:name/mods:namePart[preceding-sibling::mods:role/mods:roleTerm='translator']"></xsl:value-of></dd>
		</xsl:if>
		<!-- Show licence field only if it has a value -->
		<xsl:if test="not(mods:note[@type='licence']='')">
			<dt class="licence">Licence</dt>
			<dd class="licence"><xsl:value-of select="mods:note[@type='licence']"></xsl:value-of></dd>
		</xsl:if>
		<dt class="date_modified">Date of revision</dt>
		<dd class="date_modified">{{date_modified}}</dd>
	</xsl:template>
</xsl:stylesheet>