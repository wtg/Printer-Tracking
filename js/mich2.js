$(document).ready( function() {
	$('div#locations div.loc').click( function() {
		$('div#locations div.loc').each( function( i, obj ) {
			//if any other location is selected
			if( $(obj).hasClass( 'selected' ) ) {
				$(obj).removeClass( 'selected' ); //make it not selected
			}
			//make this objects corresponding content hidden
			var corrId = $(obj).text().replace( ' ', '-' ).toLowerCase();
			$('div#'+corrId).css( 'display', 'none' );
			//make this objects map hidden
			$('div#'+corrId+'-map').css( 'display', 'none' );
		});
		//make this location selected
		$(this).addClass('selected');
		//make this locations corresponding content showing
		corrId = $(this).text().replace( ' ', '-' ).toLowerCase();
		$('div#'+corrId).css( 'display', 'block' );
		//make this objects map showing 
		$('div#'+corrId+'-map').css( 'display', 'block' );
	});

	//slide to show details
	$('div.printer').click( function() {
		$(this).next().slideToggle( 'fast' );
	});

	//click floor selectors to filter maps
	$('div.floor-selector').click( function() {
		var mapId = $(this).text().replace( ' ', '-' );
		var mapClass = $(this).parent().attr('id');
		if( $(this).text() == 'All' ) { //show all maps
			$('div.'+mapClass).each( function( i, obj ) {
				$(obj).show();
			});
		} else {
			$('div.'+mapClass).each( function( i, obj ) {
				if( $(obj).attr('id') == mapId ) { //if user wants this map
					$(obj).show();
				} else {
					$(obj).hide();
				}
			});
		}
	});
});
