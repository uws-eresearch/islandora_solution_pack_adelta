(function ($) {


$(document).ready(function(){
if(Drupal.jsAC){
	
    var parentFound = Drupal.jsAC.prototype.found;
    Drupal.jsAC.prototype.found = function (matches) {
    	
    	if(this.input.id.match(/edit-[A-Za-z]*(-(\d+))?-adelta-namepart(--(\d+))?/)){
    	//if(this.input.id == "mint-text"){
    	
	    	// If no value in the textfield, do not show the popup.
	    	if (!this.input.value.length) {
	    		return false;
	    	}
    	
    		/*var plainMatches = {};
            for(var key in matches){
                var obj = matches[key];
                if(obj != null && typeof obj == "object"){
                    plainMatches[key] = key;
                }else{
                    plainMatches[key] = obj;
                }
            }*/
            
            // Prepare matches.
            var ul = $('<ul></ul>');
            var ac = this;
            var index;
            for (index = 0; index < matches.length; index++) {
              
            	var match = matches[index];
            	$('<li></li>')
                .html($('<div></div>').html(match['name'] + ' [' + match['uri'] + '] '))
                .mousedown(function () { ac.select(this); })
                .mouseover(function () { ac.highlight(this); })
                .mouseout(function () { ac.unhighlight(this); })
                .data('autocompleteValue', match['name'])
                .data('uri', match['uri'])
                .appendTo(ul);
            }

            // Show popup with matches, if any.
            if (this.popup) {
              if (ul.children().length) {
                $(this.popup).empty().append(ul).show();
                $(this.ariaLive).html(Drupal.t('Autocomplete popup'));
              }
              else {
                $(this.popup).css({ visibility: 'hidden' });
                this.hidePopup();
              }
            }
            
            /*$(this.popup).find("ul > li").each(function () {
            this.dataEntity = matches[$(this).data('autocompleteValue')];
        	});*/
    	}
    	else{
    		parentFound.call(this, matches);
    	}
    	
        
        
       // parentFound.call(this, plainMatches);
        
        
    };
    
    var parentHidePopup = Drupal.jsAC.prototype.hidePopup;
    Drupal.jsAC.prototype.hidePopup = function (keycode) {
       
    	//if(this.input.id == "mint-text"){
    	if(this.input.id.match(/edit-[A-Za-z]*(-(\d+))?-adelta-namepart(--(\d+))?/)){
    		
    		// Select item if the right key or mousebutton was pressed.
    		/*if (this.selected && ((keycode && keycode != 46 && keycode != 8 && keycode != 27) || !keycode)) {
    		  this.input.value = $(this.selected).data('autocompleteValue');
    		}*/
    		var entity;
    		var changed = this.selected && ((keycode && keycode != 46 && keycode != 8 && keycode != 27) || !keycode);
    		var uri_id = this.input.id.replace("adelta-namepart", "uri");
    		if (changed) {
    			entity = $(this.selected).data('uri');//this.selected.dataEntity;
    			this.input.value = $(this.selected).data('autocompleteValue');
    			
    			//put the value in the other text field. No need to trigger an event
    			//get the id using a regex. replace namepart with uri
    			$('#'+uri_id).val($(this.selected).data('uri'));
    		}
    		else{
    			$('#'+uri_id).val('');
    		}
    		// Hide popup.
    		var popup = this.popup;
    		if (popup) {
    		  this.popup = null;
    		  $(popup).fadeOut('fast', function () { $(popup).remove(); });
    		}
    		this.selected = false;
    		$(this.ariaLive).empty();
    		
    		/*if(changed){
    			var event = jQuery.Event("change");
    			//Here we set an 'entity' property in our event so we can get it at our listener.
    			event.entity = entity;
    			$(this.input).trigger(event);
    		}*/
    	}
    	else{
    		
    		parentHidePopup.call(this, keycode);
    	}
    	
    };
    
}
});

})(jQuery);