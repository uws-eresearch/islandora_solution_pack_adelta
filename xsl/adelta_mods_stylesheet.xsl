<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
xmlns:mods="http://www.loc.gov/mods/v3" xmlns:xlink="http://www.w3.org/1999/xlink" exclude-result-prefixes="mods xlink">

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
		<dt class="creator">Creator(s)</dt>
		<dd class="creator">
		<xsl:for-each select="mods:name[mods:role/mods:roleTerm='Author']">
		<span property="dc:creator">
		<xsl:attribute name="href"><xsl:value-of select="@xlink:href"/></xsl:attribute>
		<span vocab="http://xmlns.com/foaf/0.1/" typeof="Person">
		<xsl:attribute name="about"><xsl:value-of select="@xlink:href"/></xsl:attribute>
    	<span property="name"><xsl:value-of select="mods:namePart"></xsl:value-of></span>
    	</span>
    	</span><br/>
		</xsl:for-each>
		</dd>
		<xsl:if test="mods:name[mods:role/mods:roleTerm='Collaborator']">
		<dt class="collaborator">Collaborator(s)</dt>
		<dd class="collaborator">
		<xsl:for-each select="mods:name[mods:role/mods:roleTerm='Collaborator']">
		<span property="dc:contributor">
		<xsl:attribute name="href"><xsl:value-of select="@xlink:href"/></xsl:attribute>
		<span vocab="http://xmlns.com/foaf/0.1/" typeof="Person">
		<xsl:attribute name="about"><xsl:value-of select="@xlink:href"/></xsl:attribute>
    	<span property="name"><xsl:value-of select="mods:namePart"></xsl:value-of></span>
    	</span>
    	</span><br/>
		</xsl:for-each>
		</dd>
		</xsl:if>
		<dt class="unique_id">Unique id</dt>
		<dd class="unique_id"><a href="">{{unique_id}}</a></dd>
		<dt class="description">Description</dt>
		<dd class="description"><xsl:value-of select="mods:abstract[@type='description']"/></dd>
		<xsl:if test="not(mods:abstract[@type='description']/@xlink:href='')">
		<dt class="description_src">Description Source</dt>
		<dd class="description_src">
		<a><xsl:attribute name="href"><xsl:value-of select="mods:abstract[@type='description']/@xlink:href"/></xsl:attribute>
		<xsl:value-of select="mods:abstract[@type='description']/@xlink:href"/></a></dd>
		</xsl:if>
		<xsl:if test="not(mods:name/mods:namePart[preceding-sibling::mods:role/mods:roleTerm='entry_author']='')">
		<dt class="entry_author">Entry author</dt>
		<dd class="entry_author"><xsl:value-of select="mods:name/mods:namePart[preceding-sibling::mods:role/mods:roleTerm='entry_author']"/></dd>
		</xsl:if>
		<xsl:if test="not(mods:abstract[@type='artist_stmt']='')">
		<dt class="artist_stmt">Artist Statement</dt>
		<dd class="artist_stmt"><xsl:value-of select="mods:abstract[@type='artist_stmt']"/></dd>
		<dt class="artist_stmt_src">Artist statement source</dt>
		<dd class="artist_stmt_src"><xsl:value-of select="mods:abstract[@type='artist_stmt']/@displayLabel"/></dd>
		<dt class="artist_stmt_link">Artist statement link</dt>
		<dd class="artist_stmt_link"><a><xsl:attribute name="href"><xsl:value-of select="mods:abstract[@type='artist_stmt']/@xlink:href"/></xsl:attribute>
		<xsl:value-of select="mods:abstract[@type='artist_stmt']/@xlink:href"/></a></dd>
		</xsl:if>
		<dt class="language">Language</dt>
		<dd class="language"><xsl:value-of select="mods:language"/></dd>
		<dt class="date">Date</dt>
		<dd class="date"><xsl:value-of select="mods:originInfo/mods:dateIssued"/></dd>
		<!-- Show publisher field only if it has a value -->
		<xsl:if test="not(mods:originInfo/mods:publisher='')">
			<dt class="publisher">Publisher</dt>
			<dd class="publisher"><xsl:value-of select="mods:originInfo/mods:publisher"/></dd>
		</xsl:if>
		<xsl:if test="not(mods:physicalDescription/mods:note='')">
		<dt class="media">Media</dt>
		<dd class="media"><xsl:value-of select="mods:physicalDescription/mods:note"/></dd>
		</xsl:if>
		<xsl:if test="not(mods:physicalDescription/mods:internetMediaType='')">
		<dt class="internetMediaType">Medium</dt>
		<dd class="internetMediaType"><xsl:value-of select="mods:physicalDescription/mods:internetMediaType"/></dd>
		</xsl:if>
		<xsl:if test="not(mods:genre='')">
		<dt class="genre">Genre</dt>
		<dd class="genre"><xsl:value-of select="mods:genre"/></dd>
		</xsl:if>
		<xsl:if test="not(mods:location/mods:url[1]='')">
		<dt class="url">URL</dt>
		<dd class="url">
		<xsl:for-each select="mods:location/mods:url">
		<xsl:if test="not(.='')">
		<a><xsl:attribute name="href"><xsl:value-of select="."/></xsl:attribute>
		<xsl:value-of select="."/></a><br/>
		</xsl:if>
		</xsl:for-each>
		</dd>
		</xsl:if>
		<xsl:if test="not(mods:identifier[@type='isbn']='')">
			<dt class="isbn">ISBN</dt>
			<dd class="isbn"><xsl:value-of select="mods:identifier[@type='isbn']"/></dd>
		</xsl:if>
		<xsl:if test="not(mods:name/mods:namePart[preceding-sibling::mods:role/mods:roleTerm='translator']='')">
			<dt class="translator">Translator</dt>
			<dd class="translator"><xsl:value-of select="mods:name/mods:namePart[preceding-sibling::mods:role/mods:roleTerm='translator']"/></dd>
		</xsl:if>
		<xsl:if test="not(mods:note[@type='licence']='')">
			<dt class="licence">Licence</dt>
			<dd class="licence"><xsl:value-of select="mods:note[@type='licence']"/></dd>
		</xsl:if>
		<xsl:if test="not(mods:classification='')">
		<dt class="tags">Tags</dt>
		<dd class="tags"><xsl:value-of select="mods:classification"/></dd>
		</xsl:if>
		<dt class="date_modified">Date of revision</dt>
		<dd class="date_modified">{{date_modified}}</dd>
	</xsl:template>
</xsl:stylesheet>